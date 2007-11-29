#!/usr/bin/rexx


-- Testcase for ClassLoader

say ".package~test" .package~test
say ".package~asdlfkj" .package~asdlfkj
a = .Test~new
say ".Test~new" a
b = .otherclass
say ".otherclass" b
if a~class = .test then say "hooray, it worked!"

::REQUIRES "test~testpackage(someclass as somecl, otherclass)"
::REQUIRES "test"
-- uncomment to test errors in class loading
--::REQUIRES "test~test"

::CLASS Test SUBCLASS SomeCl
