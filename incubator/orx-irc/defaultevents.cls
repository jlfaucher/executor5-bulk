-- ****************************************************************************
--   Level one handler for all incoming messages.
--   This class is just a container for methods, they will be loaded
--   into a MessageHandler instance.
--
-- ****************************************************************************

/*
Warning: all methods of the DefaultEvents class have to return a value.
Methods called by .IRC~Event have to return a message object.
*/
::CLASS DefaultEvents
::METHOD Initialize
    expose client ResultArray
    use arg client
    ResultArray = .set~New
    return 0

/*
The following two methods are used to register function calles to IRC
functions. __RegisterFunctionCall registers a result object.
The method __RemoveFunctionCall removes the result object.
*/

::METHOD __RegisterFunctionCall UNGUARDED
    expose ResultArray
    use arg Object
    ResultArray~put(Object)
    return 0

::METHOD __RemoveFunctionCall UNGUARDED
    expose ResultArray
    use arg Object
    if \ ResultArray~hasindex(Object) then
	return .false
    ResultArray~remove(Object)
    return .true

/*
From this point on there are the methods that are called on incoming messages.
*/
/*
::METHOD RPL_xxx
    expose ResultArray client
    use arg message
    -- pasre the message's string
    parse value message~string with ...
    -- iterate over all entries of the ResultArray set (bad naming...)
    do entry over ResultArray
	-- filter all entries that have the right class
	if entry~class = .xxxResult then do
	    -- Here another if-statememnt could be supplied to do further verification
	    -- via the entry~Parameter value
	    -- set the result
	    entry~Result = ...
	    -- the done method sends a message to the caller
	    entry~Done
	    -- remove the function call
	    self~__RemoveFunctionCall(entry)
	    -- don't continue searching for entries
	    leave
	end
    end
    -- return the message without changing it
    return message
*/

::METHOD RPL_WHOISUSER
    expose ResultArray client
    use arg message
    parse value message~string with me nick username host '*' ':' description
    nick = client~command~Translate(nick~strip)
    do entry over ResultArray
	if entry~class = .WhoIsResult then do
	    entry~Result['DESCRIPTION'] = description
	    entry~Result['USERNAME'] = username
	    entry~Result['HOST'] = HOST
	    leave
	end
    end
    return message

::METHOD RPL_ENDOFWHOIS
    expose ResultArray client
    use arg message
    parse value message~string with me nick ':' .
    nick = client~command~Translate(nick~strip)
    do entry over ResultArray
	if entry~class = .WhoIsResult then do
	    entry~Done
	    self~__RemoveFunctionCall(entry)
	    leave
	end
    end
    return message
  
::METHOD RPL_WHOISCHANNELS
    expose ResultArray client
    use arg message
    parse value message~string with me nick ':'channels
    nick = client~command~Translate(nick~strip)
    do entry over ResultArray
	if entry~class = .WhoIsResult then do
	    if entry~Result['CHANNELS'] = .nil then
		entry~Result['CHANNELS'] = channels
	    else
		entry~Result['CHANNELS'] = entry~Result['CHANNELS'] channels
	    leave
	end
    end
    return message

::METHOD RPL_WHOISSERVER
    expose ResultArray client
    use arg message
    parse value message~string with me nick server ':'info
    nick = client~command~Translate(nick~strip)
    do entry over ResultArray
	if entry~class == .WhoIsResult then do
    	    entry~Result['SERVER'] = server
    	    entry~Result['SERVERINFO'] = info
    	    leave
	end
	--  WHOWAS also returns the whois server line
	else if entry~class == .WhoWasResult & entry~string = nick then do 
    	    entry~Result["SERVER"] = server
    	    entry~Result["INFO"] = info
    	    leave
        end
    end
    return message

-- IDLE of WHOIS reply
::METHOD RPL_WHOISIDLE
    expose ResultArray client
    use arg message
    parse value message~string with me nick idle signon ':' .
    nick = client~command~Translate(nick~strip)
    do entry over ResultArray
	if entry~class == .WhoIsResult then do
    	    entry~Result['IDLE'] = idle
    	    -- singnon is permitted by some servers
    	    entry~Result['SIGNON'] = signon
    	    leave
	end
    end
    return message


::METHOD RPL_IsOn
    expose ResultArray
    use arg message
    parse value message~string with . ':' nicks
    do entry over ResultArray
	if entry~class == .IsOnResult then do
    	    entry~Result = nicks
    	    entry~Done
	    self~__RemoveFunctionCall(entry)
	    leave
	end
    end
    return message

::METHOD RPL_Topic
    expose ResultArray client
    use arg message
    parse value message~string with . channel':' topic
    channel = client~command~Translate(channel~strip)
    do entry over ResultArray
	if entry~class = .TopicResult then do
    	    entry~Result = topic
    	    entry~Done
	    self~__RemoveFunctionCall(entry)
	    leave
	end
    end
    return message
    
::METHOD RPL_NoTopic
    expose ResultArray client
    use arg message
    parse value message~string with . channel':'
    channel = client~command~Translate(channel~strip)
    do entry over ResultArray
	if entry~class = .TopicResult then do
    	    entry~Done
	    self~__RemoveFunctionCall(entry)
	    leave
	end
    end
    return message
/*
  do i = 1 to results~items
    if results[i] = .nil then iterate
    if results[i][1] = 'TOPIC' & results[i][2] = channel then do
      topic_done = .true
    end
  end
*/
-- this reply is used by some servers but it's discarded here
-- ::METHOD RPL_ListStart
    

::METHOD RPL_LIST
    expose ResultArray
    use arg message
    parse value message~string with . channel users ':' topic
    do entry over ResultArray
	if entry~class == .ListResult then do
    	    entry~Result[entry~Result~items+1] = .array~of(channel~strip,users~strip,topic~strip)
	    leave
	end
    end
    return message

::METHOD RPL_ListEnd
    expose ResultArray
    do entry over ResultArray
	if entry~class == .ListResult then do
    	    entry~Done
	    self~__RemoveFunctionCall(entry)
	    leave
	end
    end
    return message

::METHOD RPL_Time
    expose ResultArray
    use arg message
    parse value message~string with . server ':' time
    do entry over ResultArray
	if entry~class == .TimeResult then do
	    entry~Server = server~strip
	    entry~Time   = time~strip
	    self~__RemoveFunctionCall(entry)
	    entry~Done
	    leave
	end
    end
    return message

::METHOD RPL_WhoWasUser
    expose ResultArray
    use arg message
    parse value message~string with . nick user host "*" ":" realname
    do entry over ResultArray
	if entry~class == .WhoWasResult then do
    	    entry~Result[entry~Result~items+1] = .array~of(nick~strip, user~strip,host~strip,realname~strip)
	    leave
	end
    end
    return message
  
-- Don't think we need this method, see the if ... after
-- the timoutalarm~cancel in the whowas method
-- ::METHOD Err_WasNoSuchNick 
--   expose results
--  parse arg . nick . ':' .
--  do i = 1 to results~items
--    if results[i] = .nil then iterate
--    if results[i][1] = 'WHOWAS' then do
--      results[i][2] = .nil
--    end
--  end
::METHOD RPL_EndOfWhoWas
    expose ResultArray
    use arg message
    do entry over ResultArray
	if entry~class == .WhoWasResult then do
	    self~__RemoveFunctionCall(entry)
    	    entry~Done
	    leave
	end
    end
    return message
    
::METHOD RPL_WhoReply
    expose ResultArray
    use arg message
    parse value message~string with . line
    do entry over ResultArray
	if entry~class == .WhoReply then do
    	    entry~Result[entry~Result~items+1] = line
	    leave
	end
    end
    return message
    
::METHOD RPL_EndOfWho
    expose ResultArray
    use arg message
--  parse arg ???
    do entry over ResultArray
	if entry~class == .WhoReply then do
	    self~__RemoveFunctionCall(entry)
    	    entry~Done
	    leave
	end
    end
    return message

::METHOD OnJoin
    expose ResultArray client
    use arg message
    do entry over ResultArray
	if entry~class = .JoinResult then 
	    if message~sender = client~settings~self then do
		    entry~Success = .true
		    self~__RemoveFunctionCall(entry)
		    entry~Done
		    leave
	    end
    end
    return message


--  =================================
--  DEFAULT NUMERIC REPLY METHODS
--  =================================
::METHOD OnRPL_001
    expose client
    use arg message
    parse value message~string with nick " :"
    nick = nick~strip
    if client~GetReceiverObject(nick) == .nil then
	client~settings["SELF"] = .user~new(client,nick,,
		    ,client~users,client~channels)
    client~ClientEvent('IRC.EVENT.CONNECT',client)
    return message
  -- If we get the 001 reply we can be sure that we're connected to the server sucessfully
-- message that tells us what our host is
::METHOD RPL_YourHost
    expose client
    use arg message
    parse value message~string with ":" . . . host
    client~Settings~SetEntry("Host",host)
    return message
    
-- message that contains the time the server was started
::METHOD RPL_Created
    expose client
    use arg message
    parse value message~string with ":" time
    client~Settings~SetEntry("Created",time)
    return message

-- mesage including host, verion of the server and a lot of strange info...
::METHOD RPL_MyInfo
    expose client
    use arg message
    parse value message~string with . host version
    client~Settings~SetEntry("Version",version)
    return message

-- Method parses an ISupport reply, number 005
::METHOD RPL_Isupport
    expose client
    use arg message
    parse value message~string with . modifiers
    -- lastpos is needed in this case as there may be more than one colon in
    -- the message's string
    modifiers = modifiers~left(modifiers~lastpos(":")-1)
    do i = 1 to modifiers~words
	-- each word has the form PARAMETER[=VALUE] or -PARAMETER
	-- an empty value means that the service PARAMETER is available on the server
	parse value modifiers~word(i) with token"="parameter
	if token~left(1) = "-" then do
	    -- Reset setting to default value
	    client~Settings~Revert(token~right(token~lenght-1))
	    iterate
	end
	else client~Settings~SetEntry(token,parameter)
    end
    return message

::METHOD OnRPL_UserHost
  expose users client
  use arg message
  parse value message~string with . ':' userinfostring
  do i = 1 to userinfostring~words
    parse value userinfostring~word(i) with nick'='host
    if nick~right(1) = '*' then do
      -- Nick is an IRCop
      mode = '+O'
      nick = nick~translate('','*')
    end
      else mode = ''
    awaystate = host~left(1)
    host = host~right(host~length-1)
    SELECT
      WHEN awaystate = '+' then mode = mode'-a'
      WHEN awaystate = '-' then mode = mode'+a'
      OTHERWISE Say 'Wrong 302 RPL_USERHOST from server!'
    END
--    nicko=users~getnick(nick)
--    if nicko = .nil then do
--      nicko = .nick~new(self~client,nick'!'host,users,self~client~channels)
--      nicko~mode(.msgmode~new(.nil,nicko,mode))
--      users~AddNick(nicko)
--      SAy "new user created why?" nick
--    end
--    else
--      nicko~mode(.msgmode~new(.nil,nicko,mode))
--    Say '***' nick 'is' host', mode:' mode
  end
    return message


::METHOD OnRPL_353
    expose client
    use arg message
    parse value message~string with . mode channel ':' nicks
    channelname = channel~strip
--  channel = .channel~new(client,channelname)
    channel = client~GetReceiverObject(channelname)
    if channel = .nil then channel = .channel~new(client,channelname)
    client~event(.msgjoin~new(client~settings~self,channel))
    do i = 1 to nicks~words
	nick = nicks~word(i)
	modepre = ""
	modepost = ""
	if nick~pos("@")>0 then do
	    modepre = "+o"
    	    nick = nick~translate("","@")
	    modepost = nick" "
	end
	if nick~pos("+")>0 then do
	    modepre = modepre"+v"
	    nick = nick~translate("","+")~strip
	    modepost = modepost||nick
	end
	mode = modepre modepost
	user = client~GetReceiverObject(nick)
	if user = .NIL then user = .User~new(client,nick)
	call debug "[NumHandler: OnRPL_353: Created new msgjoin for" nick "joining" channel "with mode" mode,8
        client~event(.msgjoin~new(user,channel))
	client~event(.msgMode~new(user,channel,mode))
    end
    return message

::METHOD OnRPL_433
  expose client
  use arg message
  parse value message~string with target nick .
  -- not registered yet
  if target = '*' then do 
    call debug "[NumHandler: 433: initally proposed nick is in use already.",8
--    client~event(.msgnick~new(client~users[1],nick~copies(2)~substr(2,nick~length)))
    client~write(.msgnick~new(.nil,client~settings~nick~copies(2)~substr(2,client~settings~nick~length)))
  end
    return message




::METHOD OnPing
    expose client
    use arg message
    client~write(.msgPong~new(message~string))
    return message
