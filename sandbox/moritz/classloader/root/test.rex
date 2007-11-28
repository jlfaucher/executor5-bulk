Say "Message from test.rex"
return .TestData


::CLASS TestData SUBCLASS PackageData
::METHOD Classes
    say "CLASSES"
    return .array~new
