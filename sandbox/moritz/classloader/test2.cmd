Say "Loaded!"
do c over .local_class~superclasses
  say c
end
do c over .test3~superclasses
  say c
end
::REQUIRES "test~testpackage(someclass as myclass)"
::CLASS local_class SUBCLASS "test~testpackage~otherclass"
::CLASS test3 SUBCLASS "myClass"
::CLASS test4 SUBCLASS "test~testpackage~someclass"
