
  s = .set~new
  c = .set~new
  say 's subset c' s~subset(c)

  group = .TestGroup~new("ooRexx\base\Assignments.testGroup")
  say 'group is a TestContainer:' group~isA(.TestContainer)
  say 'group has tests:         ' group~hasTests
  say 'group has test types:    ' group~hasTestTypes("UNIT")
  say 'group has test types:    ' group~hasTestTypes(.set~new)
  say 'group is empty:          ' group~isEmpty

  .environment~ooRexxUnit.class.name = "Rick"
  .local~ooRexxUnit.class.name = "Mark"

  j = readIt()

  say
  myNil = .nil
  if myNil~isA(.collection) then do
    say 'impossible, .nil is not a collection'
  end
  else do
    say 'good .nil is not a collection'
  end

::requires "ooTest.frm"
