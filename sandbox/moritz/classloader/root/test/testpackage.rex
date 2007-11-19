
return .testInfo
::CLASS TestInfo SUBCLASS PackageData
::METHOD Classes
  return .array~of(.SomeClass,.OtherClass)
::METHOD Author
  return "Moritz Hoffmann"
::METHOD Name
  return "test package"
::METHOD Version
  return 1
::METHOD License
  return "CPL"

::CLASS SomeClass

::CLASS OtherClass
