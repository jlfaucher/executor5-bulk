-- Main generic routines for comparing and other things are in here --
-- Written by Moritz Hoffmann 2006, part of the orx-irc project
-- Released under the BSD license

::CLASS routines_init
::METHOD Init CLASS
signal on syntax name err
numeric digits 10
-- the timestamps used by IRC are 10 chars long 
return
err: .debug~error(condition('O'))

::ROUTINE IRCIsChannel PUBLIC
  return (pos(left(strip(arg(1)),1),'#!+&')>0) & (length(strip(arg(1)))<= 50)

::ROUTINE IRCUpper PUBLIC
  parse arg string
  return string~translate~translate('{}|^','[]\~')
-- Use this routine to convert an IRC name to compare it

::ROUTINE IRCCompare PUBLIC
  return IRCUpper(arg(1)) = IRCUpper(arg(2))

::ROUTINE NowToTS PUBLIC
  Numeric Digits 11
  return (date('B')-719162)*86400+time('S')
-- 719162 are the days until 19700101 starting from 00010101
-- 86400 are the seconds per day 

::ROUTINE TStoStr PUBLIC
  if \ datatype(arg(1),'N') then return arg(1)
  return date('N',719162+arg(1)%86400,'B') time('N',arg(1)//86400,'S')

-- This method removes all occurances of key in string
::ROUTINE RemoveStr
    use arg string, key
    keylength = key~length
    position = string~pos(key)
    do while position > 0
        string = string~left(position-keylength)||string~right(string~length-position)
        position = string~pos(key)
    end
    return string
					
::ROUTINE MessageLocalString
    use arg message
    class = message~class
    Select
        when class = .nummsg	then return "IRC.EVENT.NUMRPL."message~number
	when class = .privmsg	then return "IRC.EVENT.PRIVMSG"
	when class = .msgnick	then return "IRC.EVENT.NICK"
	when class = .msgquit	then return "IRC.EVENT.QUIT"
	when class = .unotice	then return "IRC.EVENT.UNOTICE"
	when class = .msgkill	then return "IRC.EVENT.KILL"
	when class = .msgkick	then return "IRC.EVENT.KICK"
	when class = .msgpart	then return "IRC.EVENT.PART"
	when class = .msgtopic	then return "IRC.EVENT.TOPIC"
	when class = .msgmode	then return "IRC.EVENT.MODE"
	when class = .msgjoin	then return "IRC.EVENT.JOIN"
	when class = .invite	then return "IRC.EVENT.INVITE"
	
	when class = .msgping	then return "IRC.EVENT.PING"
	when class = .msgerror	then return "IRC.EVENT.ERROR"
	when class = .snotice	then return "IRC.EVENT.SNOTICE"
	when class = .msgwallops	then return "IRC.EVENT.WALLOPS"
	when class = .smode	then return "IRC.EVENT.SMODE"
	
	Otherwise call debug ">> MessageLocalString: Wrong message, class" class "unknown!",16
    end	

/***
** boolean = SubClassOf(class1,class2)
** This routine returns true in case class is a subclass of RefClass.
** SubClassOf(instanceOfMessage~class,.Message)
***/
::ROUTINE SubclassOf
    use arg class, RefClass
    if class = .nil then return .false
    if class = RefClass then return .true
    do subclass over class~superclasses
	if SubclassOf(subclass,RefClass) then return .true
    end
    return .false

::ROUTINE debug PUBLIC
  .local["DEBUG"]~debug(arg(1),arg(2)) -- pass messages on to the debug class

::CLASS Debug PUBLIC
::METHOD Init CLASS
  expose monitor
  monitor = .monitor~new  -- create a new monitor object to receive unknown messages

::METHOD UnInit CLASS
    self~lineout("-- Debug log ended" date() time() "--")

::METHOD Initialize CLASS
  expose monitor level
  use arg outputstream, alevel
  -- level is a char string
  if arg() < 2 then raise syntax 93.901 ARRAY (2)
  if arg() > 2 then raise syntax 93.902 ARRAY (2)
  if alevel~class = .string then
    SELECT
      WHEN alevel~datatype('NUM') then level=alevel~d2c -- convert to correct type
      WHEN alevel~length>0 then level=alevel
      OTHERWISE raise syntax 93.914 array ("level","NUM, STRING",alevel)
    END
  else raise syntax 93.914 array ("level","NUM, STRING",alevel)
  if outputstream~open("WRITE REPLACE") \= "READY:" then
    monitor~destination(.output) -- use stdout: if the output stream fails to open
  else
    monitor~destination(outputstream) -- set the monitor's destination to the stream specified
  parse source rxsource -- create some initial output of the debug log
  parse version rxversion
  self~debug("Debug Log started" date() "-" time(),255)
  self~debug(rxsource "-" rxversion,255)
-- level bits:
-- 1   = method/routine calls
-- 2   = argument input
-- 4   = returning values
-- 8   = intermediates
-- 16  = errors

  .local["DEBUG"] = self

::METHOD Unknown CLASS
  expose monitor
  forward to (monitor)


::METHOD debug CLASS
    expose level
    use arg string, aLevel
    if alevel = "" then self~lineout(">>" string)
    else if level~bitand(aLevel~d2c,"0"b) > "0"b then -- at least one bit is set...
    do
	self~lineout(aLevel~d2x~x2b||" "||string)
    end
    
::METHOD error CLASS
  use arg errobj
  -- error object are usually directories
  if errobj~class \= .directory then
    errobj = condition('O') -- no object passed, try to get a new one
  if errobj = .nil then do
    self~lineout("Error catched but no object received!")
    return
  end
  .local["IRC.DEBUG.LASTERROR"] = errobj
  self~lineout("[Debug: Error --- ERROR CATCHED! ---")
  do entry over errobj
    SELECT
      WHEN entry = "ADDITIONAL" then if errobj[entry] \= "" then if errobj[entry]~class = .array then do item over errobj[entry]
        self~lineout("> ADDITIONAL:" item)
      end
      WHEN entry = "TRACEBACK"  then if errobj[entry] \= "" then if errobj[entry]~class = .list then do item over errobj[entry]
        self~lineout("> TRACEBACK:" item)
      end
      WHEN length(errobj[entry]) = ""   then iterate
      WHEN errobj[entry] = .nil then iterate
      OTHERWISE self~lineout(">" entry":" errobj[entry])
    END
  end