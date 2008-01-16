-- Written by Moritz Hoffmann 2006, part of the orx-irc project
-- Released under the BSD license

::CLASS Channel PUBLIC

-- This method initializes a channel. When created the channel registers
-- itself with the IRC object
::METHOD Init
    expose name users client channelmode usermode
    use arg client, name
    
    -- register channel with client
    client~channels~put(self)
    
    channelmode = ""
    usermode = .table~new
    -- users is a set that stores all user objects that joined the channel
    users = .set~new
    call debug "[Channel: Init: Created" name,3

-- destroys the channel, removes itself from the IRC object
::METHOD UnInit
    expose name users client
    call debug "[Channel: UnInit:" name ,1
    client~channels~remove(self)

-- Method removes a user object from the private user list
::METHOD RemoveUser PRIVATE
    expose users usermode
    use arg nick
    users~remove(nick)
    usermode~remove(nick)
-- Method adds a user to the private user list and sets up the mode for the user
::METHOD AddUser PRIVATE
    expose users usermode
    use arg nick
    users~put(nick)
    usermode[nick] = ""

-- Method to accept events fromt the IRC server
::METHOD Event
    expose users client
    use arg message
    call debug "[Channel "self~MakeString": Event:" message~class,1
    SELECT
	when message~class=.msgjoin then do
    	    -- a new user joined the channel
    	    self~AddUser(message~sender)
    	    call debug "[Channel: Event: join message received:" message~sender~MakeString,8
    	    client~ClientEvent("IRC.EVENT.JOIN",message)
        end
	-- Part message, clean up and check if self parted
	when message~class=.msgpart then do
    	    self~removeUser(message~sender)
    	    call debug "[Channel: Event: part message received:" message~sender~MakeString,8
	    client~ClientEvent("IRC.EVENT.PART",message)
    	    if message~sender = client~settings~self then self~UnInit
	end
	-- Kick message, cleans up like the part message
	when message~class=.msgkick then do
	    self~removeUser(message~sender)
	    call debug "[Channel: Event: kick message received:" message~sender~MakeString " kicked" message~receiver~MakeString,8
    	    client~ClientEvent("IRC.EVENT.KICK",message)
	    if message~receiver = client~settings~self then do
		-- remove this channel as we just got kicked
		self~UnInit
	    end
	end
	-- Quit message
	-- there is no clean up done becauce if self quits the channel is dropped
	when message~class=.msgquit then do
    	    self~removeUser(message~sender)
    	    call debug "[Channel: Event: quit message received:" message~sender~MakeString,8
    	    client~ClientEvent("IRC.EVENT.QUIT",message)
	end
	-- like quit this message indicates a user got killed
	when message~class=.msgkill then do
    	    self~removeUser(message~sender)
    	    call debug "[Channel: Event: kill message received:" message~sender~MakeString,8
    	    client~ClientEvent("IRC.EVENT.KILL",message)
	end
	-- A mode message, parses the mode and forwards it to the client
	when message~class=.msgmode then do
	    self~SetMode(message)
    	    client~ClientEvent("IRC.EVENT.CMODE",message)
	end
	when message~class=.smode then do
	    self~SetMode(message)
    	    client~ClientEvent("IRC.EVENT.SMODE",message)
	end
	-- Message indicating a topic change
	when message~class=.msgtopic then do
    	    call debug "[Channel: Event: topic msg.   received:" message~sender~MakeString,8
    	    client~ClientEvent("IRC.EVENT.TOPIC",message)
	end
	-- a private message to all users in a channel
	when message~class=.privmsg then
	    Client~ClientEvent("IRC.EVENT.CPRIVMSG",message)
	-- a notice to all users of a channel
	when message~class=.unotice then
	    Client~ClientEvent("IRC.EVENT.CPRIVMSG",message)
	otherwise do
	    call debug "[Channel: Event: unknown message object received!" message~class,16
	end
    END

-- Method returns the name of the channel
::METHOD MakeString
    expose name; return name

-- returns true if the user 
::METHOD UserIsMember
    expose users client
    use arg nick
    if nick = .nil then return .false
    if SubclassOf(nick~class,.user) then return users~hasindex(nick)
    else return users~HasIndex(client~GetReceiverObject(nick))
        
-- returns the users set
::METHOD Users
    expose users; return users

-- returns the mode for the channel or a user of the channel
-- usage: ~GetMode or ~GetMode(nick)
::METHOD GetMode
    expose channelmode usermode
    if arg() = 0 then return channelmode
    use arg nick
    return usermode[nick]

-- Private method to parse mode strings
::METHOD SetMode PRIVATE
    expose channelmode usermode client
    use arg message
    modestr = message~string~space
    parse var modestr modifiers arguments
    paramptr = 1
    do i = 1 to modifiers~length
	char = modifiers~substr(i,1)
	SELECT
	    -- status indicates whether to switch on or disable a mode
	    when char = "+" then status = .true
	    when char = "-" then status = .false
	    -- modes that consists of a singel char
	    when char~verify("AcCDtnmMprsVzq") = 0 then
		if status then channelmode = RemoveStr(channelmode,char)||char
		    else channelmode = RemoveStr(channelmode,char)
	    -- here the banlist needs to be updated
	    when char = "b" then nop
	    -- modes the refer to a channel's user
	    when char~verify("voOh!i")= 0 then do
		nick = client~GetReceiverObject(arguments~word(paramptr))
		paramptr = paramptr + 1
		if \ usermode~HasIndex(nick) then usermode[nick] = ""
		if status then usermode[nick] = RemoveStr(usermode[nick],char)||char
		    else usermode[nick] = RemoveStr(usermode[nick],char)
	    end
	    otherwise call debug "[Channel: SetMode: unknown modifier" char,8
	END
    end
    