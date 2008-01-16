-- This is the main class file
-- Written by Moritz Hoffmann 2006, part of the orx-irc project
-- Released under the BSD license

-- The following lines will be kept and shop up as the beginning lines of the
-- final irc.cls file.
/****************************************************************************
** This is the object rexx IRC interface class
** Written by Moritz Hoffmann, 2005-2006, Released under the BSD license
** If you find just one file called irc.cls you have a compiled version
** of the source. Go to sf.net/projects/orx-irc to get the full source
** and the documentation.
** There is a subversion tree a well available on:
**      http://svn.netlabs.org/repos/orx-irc/
** This is not a stand alone application; it only offers an interface for
** other applications.
**
** VERSION: 0.1-rc1
****************************************************************************/


-- *************************************************************************
-- ** The IRC Class
-- ** This is the main class of the irc client. All communication with the server
-- ** passes the class
-- *************************************************************************
-- preserve the time the execution started
.LOCAL["IRC.STARTTIME"] = Time("Reset")

::CLASS irc PUBLIC
-- initialization, accepts a host object
::METHOD Init
    expose users channels MessageHandler ClientEvent TalkBack
    call debug "[IRC: Init: Starting to initialize new object...",1
    -- an array that contains the current user objects, ID is the position in the array
    users = .set~new
    -- an array that contains the current active channel objects and their ID  
    channels = .set~new

-- The following lines won't be needede anymore as all incoming messages are
-- forwarded to the ClientEvent object. The methods used to analyze the
-- numeric replies will be loaded by default.

--    -- a handler to handle incoming numeric messages
--    NumHandler = .NumHandler~new(self)

    -- create the level one message handler object
    MessageHandler = .MessageHandler~new
    MessageHandler~EventSetMethods(.DefaultEvents~Methods)
    MessageHandler~Initialize(self)
    -- initialize the ClientEvent object
    ClientEvent = .ClientEvent~new

    -- create the talkback object
--    TalkBack = .ClientTB~New(self,NumHandler)

    self~Command = .IRCCommands~new(self)

    -- a directory to hold specific information about the IRC server
    self~settings = .Issupported~new

    -- debug that the class has been created succesfully
    call debug "[IRC instance created]",1

    call Debug "Time elapsed:" Time("Elapsed"),1
    -- return the current instance
    return self

-- make the users array public
::METHOD Users    ATTRIBUTE UNGUARDED

-- make the channels object public
::METHOD Channels ATTRIBUTE UNGUARDED

-- give access to the Talkback object
-- ::METHOD TalkBack UNGUARDED
--    expose TalkBack; return TalkBack

::METHOD MessageHandler UNGUARDED
    expose MessageHandler; return MessageHandler

-- grant access to the IRC command handler
::METHOD Command ATTRIBUTE UNGUARDED

-- make the settings accessible
::METHOD Settings ATTRIBUTE
    
-- method connect connects to an IRC server
-- call: ~connect(host object,password,default nick name, user name, comment)
::METHOD Connect
    expose host serverpass socket
    -- tell the client the object has been initialized
    self~ClientEvent("irc.event.init",self)

    -- get the arguments
    use arg host, serverpass, nick, username, description
    self~settings["NICK"] = nick
    self~settings["USERNAME"] = username
    
    call debug "[IRC: Connect method called]",1
    -- create a new line based socket
    socket = .LNSOCKET~new(host,self)
    -- now connect
    rc = socket~connect
    if rc \= 0 then do
	call debug "[IRC: Connect: socket return value:" rc", error connecting!", 16
        return
    end
    call debug "[IRC: Connect: socket connected]",8
    call SysSleep 0.5
    if serverpass \= .nil then do
	call debug "[IRC: Connect: PASS command will be written]",8
	self~write(.msgpass~new(serverpass))
--	self~writeln('PASS '||serverpass)
    end
    self~write(.msgnick~new(nick,nick))
--    self~writeln('NICK' nick)
    self~write(.msguser~new(username,7,description))
--    self~writeln('USER' username '7 * :'description)
    call debug "[IRC: Connect: NICK and USER command written]",8
--  .user~new(self,nick) -- creates user with ID=1, the own user
-- the connect event is created by num rpl 001 to make sure the connection is up
-- when the command is issued
--    self~ClientEvent("irc.event.connect")

::METHOD DisConnect
    expose socket
    if socket~connected then do
	msg = .msgQuit~new(arg(1),self~settings~self)
	self~Write(msg)
	call debug "[IRC: DisConnect: Disconnecting |"arg(1)"|]",1
        socket~disconnect
        self~ClientEvent("IRC.EVENT.DISCONNECT",msg)
    end
    else call debug "[IRC DisConnect: Not connected, can not disconnect",16

::METHOD Connected UNGUARDED
    expose socket
    return socket~connected

-- This method writes a string on the socket. Please not that the use of
-- this function is depricated. Create a messsage object instead and use
-- the Write method of this class to send the message to the server.
::METHOD Writeln 
    expose socket
    call debug "[IRC: WriteLN:" arg(1),3
    call debug "[IRC: WriteLN is depricated!",3
    socket~write(arg(1)||"0d0a"x)
    self~ClientEvent("IRC.EVENT.WRITE",arg(1))

-- This method writes a message object to the socket. The message is only
-- written in case the object is an instance of the ircmessage class.
-- returns true for success, false otherwise.
::METHOD Write UNGUARDED
    expose socket
    use arg message
    if message = .nil then return .false
    call debug "[IRC: Write:" message~class||" "||message~IRCString,3
    if SubclassOf(message~class,.ircmessage) then do
	socket~write(message~IRCString||"0d0a"x)
        self~ClientEvent("IRC.EVENT.WRITE",arg(1))
	return .true
    end
    call debug "[IRC: Write: failed to write message, message is no subclass of .ircmessage!",16
    return .false
        
::METHOD GetEventHandler UNGUARDED
    expose ClientEvent
    return ClientEvent

-- This method forwards events to the client
::METHOD ClientEvent UNGUARDED
    expose ClientEvent
--    trace r
    signal on syntax name err
    -- the argument count is 2, meaning the second parameter was supplied,
    -- which is the event's parameter
    if arg() = 2 then do
	-- get the name and message argument
	use arg name,message
        eventmessage =.message~new(ClientEvent,name,'I', message)~~send
    end
    -- only the name was supplied
    else do
	use arg name
        eventmessage =.message~new(ClientEvent,name)~~send
    end
    return
    
    -- forward all errors to the debug object to protect the main thread from
    -- terminating
    err:
    .debug~error(condition('O'))




-- This message gets called whenever there is new input from the socket
::METHOD Available
    -- add a blank line to the debug output to improve readability
    call debug ""time(),1
    call debug "[IRC: Available: |"arg(1), 3
    -- forward the incoming string to the private parseline method
    message = self~StrToMessage(arg(1))
    -- if a message was returned it needs to be handled by the local event handler
    if message \= .nil then self~Event(message)

-- this method receives all events that come from the server
::METHOD Event UNGUARDED
    expose MessageHandler
    signal on syntax name err
    use arg message
-- ******************************
--     This method receives the messages created by the message parser. All
--     transformed messages that arrive pass through this method.
--     To forward the messages to their appropriate receiver objects, the
--     superclasses of the messages are analyzed.
--     For now, there's a receiver object for numerc replies.
-- ********************************
    call debug TSToStr(message~Timestamp) "[IRC: Event:" message~class,3
    if message= .nil then raise syntax 93.911
    -- first pass the message on to the level one message handler
    -- the message handler has to return a message
    event = .Message~New(MessageHandler,MessageLocalString(message),"I",message)~~send
    message = event~Result

    superclasses = message~class~superclasses
    -- filter numeric replies
    if message~class = .nummsg then do
	self~ClientEvent("IRC.EVENT.NUMRPL."message~number,message)
	return
    end
    -- here all other messages get forwarded to their receiver
    else do superclass over superclasses
	-- filter for server messages
	if superclass = .smsg then do -- filter for .smsg superclass
	    SELECT
		when message~class = .msgerror then do
		    call debug "[IRC: Event: Error received, disconnecting!",9
		    self~disconnect
		    return
		end
--    	    	when message~class = .msgping then self~writeln("PONG :"message~string)
		when message~class = .snotice then self~ClientEvent("IRC.EVENT.SNOTICE",message)
    		otherwise nop
    	    END
    	    return
	end
	if superclass = .umsg then do
    	    message~sender~event(message)
    	    return
	end
	if superclass = .umsgr then do
    	    message~sender~event(message)
    	    return
	end
    end
    return
    
    err:
    .debug~error(condition('O'))

-- This is the parsing method
-- It converts the incoming string to it's appropriate message object
-- and returns it. If it failed .nil is returned instead.
::METHOD StrToMessage UNGUARDED
    expose events channels users
    call debug "[IRC: ParseLine",1
    parse arg line
    parse var line prefix command params
    params=params~strip
    command = command~strip

    -- there was no prefix, the prefix *always* starts with a colon
    if prefix~left(1) \= ':' then do
	params = command
	command = prefix
	prefix = ''
	origin='SERVER'
    end
    else do
	prefix = prefix~right(prefix~length-1)~strip
	parse var prefix nick'!'username'@'host
	if nick~pos('.') = 0 then do
	-- Message is from a server, nicks never contain dots
	-- This workaround fails if the server name doesn't contain dots...
	origin = 'NICK'
    end
	else origin = 'SERVER'
    end
    message=.nil
    call debug "[IRC: ParseLine: origin='"origin"'",8
    if origin = 'SERVER' then
	-- it's a numeric reply
	if command~datatype = 'NUM' then message=.nummsg~new(command,params)
	else do
	Select
	when command = 'PING' then do
	    parse var params . ':'ping_str
    	    message=.msgping~new(ping_str)
	end
	WHEN command = 'ERROR' then message=.msgerror~new
	WHEN command = 'NOTICE' then do
	    if prefix = '' then parse var line ':' text
    	    else parse var params ':' text
    	    message=.snotice~new(text)
	end
        WHEN command = 'WALLOPS' then do
    	    if prefix = '' then parse var line ':' text
    	    else parse var params ':' text
    	    message=.msgwallops~new(text)
	end
	WHEN command = "MODE" then do
    	    if prefix = '' then parse var line . target modifier
    	    else parse var params target modifier
    	    message = .smode~new(prefix,self~GetreceiverObject(target),modifier)
	end
	otherwise nop
	END
    end
    else do
	nickobject = self~GetReceiverObject(nick)
	if nickobject = .nil then do
	    nickobject = .user~new(self,prefix,users,channels)
	end
	if (\ nickobject~hashostmark) & (username <> '') & (host <> '') then do
	    nickobject~SetHostMark(nickobject~makestring'!'username'@'host)
	end
	parse var params commands ':'last
	commands = commands~strip; last=last~strip
	SELECT
	WHEN command = 'NICK'    then message= .msgnick~new(nickobject,last)
	WHEN command = 'QUIT'    then message= .msgquit~new(nickobject,last)
	WHEN command = 'KILL'    then do
	    message= .msgkill~new(nickobject, self~GetReceiverObject(commands), last)
	end
	WHEN command = 'JOIN'    then message= .msgjoin~new(nickobject,last)
	WHEN command = 'PART'    then message= .msgpart~new(nickobject,self~GetReceiverObject(commands),last)
	WHEN command = 'KICK'    then do
    	    parse value commands~space with channel targetnick
    	    message=.msgkick~new(nickobject,self~GetReceiverObject(targetnick),self~GetReceiverObject(channel),last)
	end
        WHEN command = 'TOPIC'   then do
    	    message= .msgtopic~new(nickobject,self~GetReceiverObject(commands),last)
        end
        WHEN command = 'MODE'    then do
	    parse var params receiver string
            message= .msgmode~new(nickobject,self~GetReceiverObject(receiver),string)
        end
        WHEN command = 'PRIVMSG' then do
    	    message= .privmsg~new(nickobject,self~GetReceiverObject(commands),last)
        end
        WHEN command = 'NOTICE'  then do
    	    message= .unotice~new(nickobject,self~GetReceiverObject(commands),last)
        end
        WHEN command = 'INVITE'  then do
            message= .invite~new(nickobject,self~GetReceiverObject(commands),last)
        end
        OTHERWISE call debug "[IRC: ParseLine:" nickobject~nick 'sent unknown command' command '(params='params')',24
	END
    end
    return message

-- this message returns a channel or nick object if it's known or the nil object
::METHOD GetReceiverObject UNGUARDED
    expose users channels
    parse arg string
    call debug "[IRC: GetReceiverObject:" string,3
    string = self~command~Translate(string)
    do user over users
	
	if self~command~Translate(user~MakeString) = string then do
	    call debug "[IRC: GetReceiverObject: found" string,4
	    return user
	end
    end
    do channel over channels
	if self~command~Translate(channel~MakeString) = string then do
	    call debug "[IRC: GetReceiverObject: found" string "id:" i,4
	    return channel
	end
    end
    call debug "[IRC: GetReceiverObject:" string "not found, returning NIL.",4
    return .nil


