-- prolog code.  This should be called after all of the activate methods
-- are called, so add some checks to raise an error.

-- if there was some other assertion failure, don't overwrite the pending error
if .local~class.testgroup~assertFail \= .true then do
  if .local~class.testgroup~class1 \= .true then call setError "class1 not activated before prolog"
  if .local~class.testgroup~class2 \= .true then call setError "class2 not activated before prolog"
  if .local~class.testgroup~class3 \= .true then call setError "class3 not activated before prolog"
end

-- the subclassing here will switch the install order around
::class class1 subclass class3
::method init class
  nop
::method activate class
  instance = self~new
  -- this should not give an error
  instance~foo
  .local~class.testgroup~class1 = .true
  if .local~class.testgroup~class3 \= .true then
      call setError "class3~activate did not run before class1~activate"
  if .local~class.testgroup~class2 \= .true then
      call setError "class1~activate ran before class2~activate"

  if \.class1~isa(.class) then call setError ".class1 not available in class1~activate"
  if \.class2~isa(.class) then call setError ".class2 not available in class1~activate"
  if \.class3~isa(.class) then call setError ".class3 not available in class1~activate"

::method foo
  return .true

-- this class will run first because this is the first class without a dependency
::class class2
::method init class
  nop
::method activate class
  .local~class.testgroup~class2 = .true
  if .local~class.testgroup~class3 == .true then
      call setError "class3~activate ran before class2~activate"
  if .local~class.testgroup~class1 == .true then
      call setError "class1~activate ran before class2~activate"

  if \.class1~isa(.class) then call setError ".class1 not available in class2~activate"
  if \.class2~isa(.class) then call setError ".class2 not available in class2~activate"
  if \.class3~isa(.class) then call setError ".class3 not available in class2~activate"

-- this class will run first because of the subclass dependency of class1
-- dependency order
::class class3
::method init class
  nop
::method activate class
  .local~class.testgroup~class3 = .true
  if .local~class.testgroup~class1 == .true then
      call setError "class3~activate did not run before class1~activate"
  if .local~class.testgroup~class2 \= .true then
      call setError "class3~activate ran before class2~activate"

  if \.class1~isa(.class) then call setError ".class1 not available in class3~activate"
  if \.class2~isa(.class) then call setError ".class2 not available in class3~activate"
  if \.class3~isa(.class) then call setError ".class3 not available in class3~activate"

::routine setError
  use strict arg reason

  .local~class.testgroup~assertFail = .true
  .local~class.testgroup~assertFailReason = reason


::options novalue error
