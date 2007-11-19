#!/usr/bin/rexx


-- Testcase for ClassLoader


a = .Test~new
say a
b = .otherclass
say b
if a~class = .test then say "hooray, it worked!"

::REQUIRES "test~testpackage(someclass as somecl, otherclass)"

::CLASS Test SUBCLASS SomeCl
