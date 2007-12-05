
  parse source . . s
  group = .TestGroup~new(s)

  say 'group is a .TestGroup:' group~isA(.TestGroup)
  say '.TestGroup is a .TestGroup:' .TestGroup~isA(.TestGroup)
  say '.TestGroup is a subclass of .TestGroup:' isSubClassOf( .TestGroup, "TestGroup")
  say 'group is a subclass of .TestGroup:' isSubClassOf( group, "TestGroup")


::requires "ooTest.frm"
