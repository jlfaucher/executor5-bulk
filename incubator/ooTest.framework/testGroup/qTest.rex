
  s = .set~new
  c = .set~new
  say 's subset c' s~subset(c)

  cData = .ConditionData~new(timeStamp(), "E:\file", 'BOGUS')
  say 'cData when:    ' cData~when
  say 'cData where:   ' cData~where
  say 'cData type:    ' cData~type
  say 'cData c object:' cData~conditionObject
  say

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

  say 'Query super classes of TestGroup:'
  j = querySuperClasses(.TestGroup)

  say 'TestGroup is a subclass of TestContainer:' .TestGroup~isSubClassOf(.TestContainer)
  say 'TestGroup is a TestContainer:            ' .TestGroup~isA(.TestContainer)
  say 'TestGroup is a Class:                    ' .TestGroup~isA(.Class)
  say 'group is a Class:                        ' group~isA(.Class)
  say 'group is a TestContainer:                ' group~isA(.TestContainer)
  if group~isA(.class) then do
    say 'group is a subclass of TestContainer:    ' group~isSubClassOf(.TestContainer)
  end

  cObj = 'doIt'('TestGroup')
  say 'cObj is a class:' cObj~isA(.class)
  say 'cObj is a subclass of TestContainer:' cObj~isSubClassOf(.TestContainer)

  obj = 'doIt'('group')
  say 'obj is a class:        ' obj~isA(.class)
  say 'obj is a TestGroup:    ' obj~isA(.TestGroup)
  say 'obj is a TestContainer:' obj~isA(.TestContainer)

::requires "ooTest.frm"

::routine readIt
  say 'class.name:' .ooRexxUnit.class.name
return 0
