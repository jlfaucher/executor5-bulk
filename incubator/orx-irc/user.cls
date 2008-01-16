-- Written by Moritz Hoffmann 2006, part of the orx-irc project
-- Released under the BSD license

::CLASS user PUBLIC
::METHOD init
    -- A Nick Object: Give the hostmark and the server's nicklist as arguments 
    expose nick username host client mode
    use arg client,hostmark
    parse var hostmark nick'!'username'@'host
    self~hashostmark = \(username = '' & host = '')

    -- register self with the client argument
    client~users~put(self)
    
    -- store the own IRC user :-)
    if \ client~settings~HasIndex("SELF") then
	client~settings["SELF"] = self
	
    -- the mode string holds the currently active mode settings
    mode = ""
    call debug "[USER: nick "nick" created",1

::METHOD UnInit
  expose client
  client~users~remove(self);

::METHOD MakeString -- returns the nick as string
  expose nick
  return nick

::METHOD Event
    expose client
    use arg message
    call debug "[User" self~makestring":" message~class,3
--  say (message~class == .msgmode)||message~class||" == "||.msgmode
    SELECT
    when message~class=.msgnick then do
	call debug "[User: Event: MsgNick:" self~nick "changed nickname to" message~nick,8
	-- set the nick string to the new value
	self~nick = message~nick
	client~ClientEvent("IRC.EVENT.NICK",message)
    end
    when message~class=.msgjoin then do
	call debug "[User: Event MsgJoin from" message~sender "to" message~receiver,8
        channel = client~GetReceiverObject(message~receiver)
	if channel = .nil then channel = .channel~new(client,message~receiver)
	channel~event(message)
    end
    when message~class=.msgpart then do
	call debug "[User: Event: MsgPart:" message~sender "to" message~receiver,8
	message~receiver~event(message)
	-- in case the current user object is not the client it has to be dropped if
	-- it has no more channels associated
	if self \= client~settings~self then do
	    i = .true
	    do channel over client~channels
		if client~UserIsMember(message~sender) then do
		    i = .false; leave
		end
	    end
	    if i then do
		self~uninit
	    end
	end
    end

    when message~class=.msgkick then do
	call debug "[User: Event: MsgKick:" message~sender "kicked" message~receiver "from" message~channel,8
	if message~sender = client~settings~self then do  -- own user kicked another user from some channel
    	    call debug "[User: Event: MskKick:" self~nick "is target!",8
    	    message~receiver~event(message) -- forward the message to the user who got kicked
        end
        else do  -- user got kicked
    	-- forward kick message to channel
	    message~channel~event(message)
	end
    end
    when message~class=.msgquit then do
	call debug "[User: Event: MsgQuit:" message~sender
	-- remove user from channels
	do channel over client~channels
	    if channel~UserIsMember(message~sender) then channel~event(message)
	end
	client~ClientEvent("IRC.EVENT.QUIT",message)
	self~uninit
    end
    when message~class=.msgkill then do
	call debug "[User: Event: MsgKill:" message~sender~MakeString "killed" message~receiver~Makestring
	if message~sender = self then message~receiver~Event(message)
	else do
		-- remove user from channels
	    do channel over client~channels
		if channel~UserIsMember(message~sender) then channel~event(message)
	    end
	    client~ClientEvent("IRC.EVENT.KILL",message)
	    self~uninit
	    drop self
	end
    end
    when message~class=.msgmode then do
--	say "Mode received"
	if message~receiver \= self then do
	    -- forward message to receiver (channel probably)
	    message~receiver~Event(message)
        end
	else do
	    self~mode(message)
	    client~ClientEvent("IRC.EVENT.UMODE",message)
	end
    end
    when message~class=.privmsg then do
	call debug ">"message~sender~MakeString"<" message~receiver~MakeString message~string,8
	-- check if the user object send the privmsg
	if message~sender = self then
	    -- forward the message to the receiver
	    message~receiver~Event(message)
	else
	    -- here's the message's path end
	    client~ClientEvent("IRC.EVENT.UPRIVMSG",message)      
    end
    when message~class=.unotice then do
	call debug "-"message~sender~MakeString"-" message~receiver~MakeString message~string,8
	-- check if the user object send the privmsg
	if message~sender = self then
	    -- forward the message to the receiver
	    messge~receiver~Event(message)
	else
	    -- here's the message's path end
	    client~ClientEvent("IRC.EVENT.UNOTICE",message)      
    end
    when message~class=.invite then do
	-- check if the user object send the privmsg
	if message~sender = self then
	    -- forward the message to the receiver
	    message~receiver~Event(message)
	else
	    -- here's the message's path end
	    client~ClientEvent("IRC.EVENT.INVITE",message)      
    end
    otherwise call debug "[USER: Event:" self~nick': Unknown message received' message~string,16
  END


::METHOD Nick
  expose nick;return nick
::METHOD "Nick="
  expose nick; use arg nick
::METHOD username ATTRIBUTE
::METHOD host ATTRIBUTE

::METHOD hostmark
  return self~nick'!'self~username'@'self~host

::METHOD HasHostMark Attribute

::METHOD SetHostMark
  expose nick username host
  parse arg hostmark
  parse var hostmark nick'!'username'@'host
  self~hashostmark = \ (username = '' & host = '')


::METHOD Mode
    expose mode;return mode

::METHOD SetMode PRIVATE
    expose channels mode client
    use arg message
    modestr = message~string~space
    -- Update user's mode directory
    -- Mode parsing algorithm adapted from ircd's note.c, function set_flags
    if modestr~pos(':')= 1 then
	parse value modestr with ':'modchange params
    else
	parse value modestr with modchange params

    paramptr=1
    do i = 1 to modchange~length
      char=modchange~substr(i,1)
      if char = '' then leave
      SELECT
        WHEN char = '+' then state = .true
        WHEN char = '-' then state = .false
        OTHERWISE do
	    if state then mode = RemoveStr(mode,char)
	    else mode = RemoveStr(mode,char)||char
	end
      END
    end
