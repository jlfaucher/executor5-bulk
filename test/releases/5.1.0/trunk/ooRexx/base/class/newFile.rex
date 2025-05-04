-- used to test newFile() method in Method.testGroup and Routine.testGroup

  defined = ""
  if .context~package~findPublicClass("TSTCLASSPUBLIC") \= .nil then
    defined = defined .TstClassPublic~TstMethod
  if .context~package~findPublicRoutine("TSTROUTINEPUBLIC") \= .nil then
    defined = defined TstRoutinePublic()
  if .context~package~findClass("TSTCLASS") \= .nil then
    defined = defined .TstClass~TstMethod
  if .context~package~findRoutine("TSTROUTINE") \= .nil then
    defined = defined TstRoutine()
  return defined~strip("L")

