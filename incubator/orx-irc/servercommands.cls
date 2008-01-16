/**
Sometimes it is needed for an IRC client to send commands to a server. In
that case the server returns a message whether the command was successful
or not.
Imagine the following scenario: A script wants to join a new channel. To
do this it creates a MsgJoin object with the channelname as argument. Now
it calles a method of a class to send the message to the server.
The message will return something in all cases. Either the server will
return the join message or it will send a numeric reply mentioning the reason
why it was not possible to join the channel. In both cases the calling
script should be informed about the state after issuing the command.
Of course it would be possible to scan the channel list after issuing the
command, but that is not reliable (think about timing for example).
To avoid that problem the script is forced to call a method that registers
the message issued in an internal handler. This handler processes all input
from the server, meaning that the messages indicating success or failure
can be catched as well. Now the handler executes a method registered to
tell the caller that a result is available.
At this point the script continues running. That means that the join
method call blocks the execution of the script until a result is received.
To avoid dead-locks there is a timeout for calls.
*/

::CLASS Result MIXINCLASS Object
::METHOD Init
    expose message parameter completed
    use arg message, parameter
    self~Success = .false
    Completed = 0
    
::METHOD Done UNGUARDED
    expose message
    Completed = NowToTS()
    message~send
::METHOD Parameter
    expose parameter
    return parameter
::METHOD Result ATTRIBUTE
::METHOD Success ATTRIBUTE
::METHOD Completed
    expose completed
    return completed
::METHOD Message ATTRIBUTE

::CLASS WhoResult	SUBCLASS ircmessage INHERIT Result
::METHOD Init
    self~Result = .array~new
    forward class (super)
::METHOD IRCString
    return "WHO "||self~Parameter

::CLASS WhoIsResult	SUBCLASS ircmessage INHERIT Result
::METHOD Init
    self~Result = .Directory~new
    forward class (super)
::METHOD IRCString
    return "WHOIS "||self~Parameter

::CLASS WhoWasResult	SUBCLASS ircmessage INHERIT Result
::METHOD Init
    Self~Result = .array~new
    forward class (super)
::METHOD IRCString
    return "WHOWAS "||self~Paramter
    
::CLASS IsOnResult	SUBCLASS ircmessage INHERIT Result
::METHOD IRCString
    return "ISON "||self~Parameter

-- channel message results
::CLASS JoinResult	SUBCLASS ircmessage INHERIT Result
::METHOD Init
    expose key
    if arg() = 3 then key = arg(3)
    else key = ""
    forward class (super) array (arg(1),arg(2))
::METHOD IRCString
    expose key
    return "JOIN "||self~Parameter||" "||key
    
::CLASS PartResult	SUBCLASS ircmessage INHERIT Result
::METHOD IRCString
    return "PART "||self~Parameter

::CLASS ChannelModeResult	SUBCLASS ircmessage INHERIT Result
::METHOD IRCString
    return "MODE "||self~Parameter

::CLASS TopicResult	SUBCLASS ircmessage INHERIT Result
::METHOD IRCString
    return "TOPIC "||self~Parameter
::METHOD Identifier
    return self~Parameter
    
::CLASS NamesResult	SUBCLASS ircmessage INHERIT Result
::METHOD IRCString
    return "NAMES "||self~Parameter

::CLASS ListResult	SUBCLASS ircmessage INHERIT Result
::METHOD Init
    self~Result = .array~new
    forward class (super)
::METHOD IRCString
    return "LIST "||self~Parameter

::CLASS KickResult	SUBCLASS ircmessage INHERIT Result
::METHOD IRCString
    return "KICK "||self~Parameter

-- Connection message results
::CLASS PassResult	SUBCLASS ircmessage INHERIT Result
::METHOD IRCString
    return "PASS "||self~Parameter

::CLASS NickResult	SUBCLASS ircmessage INHERIT Result
::CLASS UserResult	SUBCLASS ircmessage INHERIT Result
::CLASS OperResult	SUBCLASS ircmessage INHERIT Result
::CLASS UserModeResult	SUBCLASS ircmessage INHERIT Result

-- Service related commands
::CLASS ServListResult	SUBCLASS ircmessage INHERIT Result
::CLASS SQueryResult	SUBCLASS ircmessage INHERIT Result

-- Server based querys & commands
::CLASS ModtResult	SUBCLASS ircmessage INHERIT Result
::CLASS LUsersResult	SUBCLASS ircmessage INHERIT Result
::CLASS VersionResult	SUBCLASS ircmessage INHERIT Result
::CLASS StatsResult	SUBCLASS ircmessage INHERIT Result
::CLASS LinksResult	SUBCLASS ircmessage INHERIT Result

::CLASS TimeResult	SUBCLASS ircmessage INHERIT Result
::METHOD Server ATTRIBUTE
::METHOD Time ATTRIbUTE
::METHOD IRCString
    return "TIME "||self~Parameter
    
::CLASS ConnectResult	SUBCLASS ircmessage INHERIT Result
::CLASS TraceResult	SUBCLASS ircmessage INHERIT Result
::CLASS AdminResult	SUBCLASS ircmessage INHERIT Result
::CLASS InfoResult	SUBCLASS ircmessage INHERIT Result

-- Additional features
::CLASS AwayResult	SUBCLASS ircmessage INHERIT Result
::CLASS RehashResult	SUBCLASS ircmessage INHERIT Result
::CLASS DieResult	SUBCLASS ircmessage INHERIT Result
::CLASS RestartResult	SUBCLASS ircmessage INHERIT Result
::CLASS SummonResult	SUBCLASS ircmessage INHERIT Result
::CLASS UsersResult	SUBCLASS ircmessage INHERIT Result
::CLASS OperwallResult	SUBCLASS ircmessage INHERIT Result
::CLASS UserhostResult	SUBCLASS ircmessage INHERIT Result


::CLASS IRCCommands
::METHOD Init
    expose client
    use arg client
    -- raise something in case client is no subclass of .irc...

-- ================ WHOIS       =================================
::METHOD WhoIs
    expose client whois_done
    -- the server needs a nickname string, thus parse arg is used here
    parse arg nick
    -- leave that for now, is likly to be changed later
    nick = self~Translate(nick~strip)
    -- initialize the status variable
    whois_done = .false
    
    /*
    The result object now needs to be registered with the level one
    message handler. The first parameter is the method that is to be
    called when the result is received, the second one is message specific.
    */
    -- Create a result object
    message = .WhoIsResult~new(.message~new(self, 'WHOISCANCEL'), nick)
    -- send the command to the server
    client~write(message)
    -- register the object and store the ID
    client~MessageHandler~__RegisterFunctionCall(message)
    -- start the alarm
    timeoutalarm = .alarm~new(.local['IRC.TIMEOUT.SCRIPTS'], .message~new(self, 'WHOISCANCEL'))
    -- wait until the status variable is no more .false
    guard off when whois_done \= .false
    -- cancle the alarm; has no effect in case the alarm has gone off
    timeoutalarm~cancel
    -- clean up
    client~MessageHandler~__RemoveFunctionCall(message)
    -- finally return the data (stored in the result object)
    return message

::METHOD WhoIsCancel
  expose whois_done
  whois_done = .true

-- ================ ISON        =================================
::METHOD IsOn
    expose ison_done client
    parse arg nicks
    ison_done = .false
    message = .IsOnResult~new(.message~new(self, "ISONCANCEL"),nicks) 
    client~write(message)
    client~MessageHandler~__RegisterFunctionCall(message)
    timeoutalarm = .alarm~new(.local['IRC.TIMEOUT.SCRIPTS'], .message~new(self, 'IsOnCancel'))
    guard off when ison_done \= .false
    timeoutalarm~cancel
    client~MessageHandler~__RemoveFunctionCall(message)
    return message
    
::METHOD IsOnCancel
  expose ison_done
  ison_done = .true

-- ================ JOIN        =================================
-- Joins a channel
::METHOD Join
    expose join_done client
    parse arg channel, key
    join_done = .false
    message = .JoinResult~new(.message~new(self, "JOINCANCEL"),channel,key) 
    client~write(message)
    client~MessageHandler~__RegisterFunctionCall(message)
    timeoutalarm = .alarm~new(.local['IRC.TIMEOUT.SCRIPTS'], .message~new(self, 'JoinCancel'))
    guard off when join_done \= .false
    timeoutalarm~cancel
    client~MessageHandler~__RemoveFunctionCall(message)
    return message
    
::METHOD JoinCancel
  expose Join_done
  Join_done = .true

-- ================ TOPIC       =================================
-- Returns the topic of a channel or a null string
::METHOD Topic
    expose topic_done client
    parse arg channel
    topic_done = .false
    message = .TopicResult~new(.message~new(self, "TOPICCANCEL"),nicks) 
    client~write(message)
    client~MessageHandler~__RegisterFunctionCall(message)
    timeoutalarm = .alarm~new(.local['IRC.TIMEOUT.SCRIPTS'], .message~new(self, 'TopicCancel'))
    guard off when topic_done \= .false
    timeoutalarm~cancel
    client~MessageHandler~__RemoveFunctionCall(message)
    return message

::METHOD TopicCancel
  expose topic_done
  topic_done = .true
-- ================ LIST        =================================
-- returns a 2 dimensional array. array[n]=[channel,users,topic]
::METHOD List 
    expose list_done client
    parse arg channels
    channels = self~Translate(channels~translate(" ",",")~space)
    list_done = .false
    message = .ListResult~new(.message~new(self, "LISTCANCEL"),channels) 
    client~write(message)
    client~MessageHandler~__RegisterFunctionCall(message)
    timeoutalarm = .alarm~new(.local['IRC.TIMEOUT.SCRIPTS'], .message~new(self, 'ListCancel'))
    guard off when list_done \= .false
    timeoutalarm~cancel
    client~MessageHandler~__RemoveFunctionCall(message)
    return message


::METHOD ListCancel
    expose list_done
    list_done = .true
-- ================ TIME        =================================
::METHOD Time
    expose time_done client
    parse arg server
    time_done = .false
    message = .TimeResult~new(.message~new(self, "TIMECANCEL"),server) 
    client~write(message)
    client~MessageHandler~__RegisterFunctionCall(message)
    timeoutalarm = .alarm~new(.local['IRC.TIMEOUT.SCRIPTS'], .message~new(self, 'TimeCancel'))
    guard off when Time_done \= .false
    timeoutalarm~cancel
    client~MessageHandler~__RemoveFunctionCall(message)
    return message

::METHOD TimeCancel
    expose time_done
    time_done = .true

-- ================ WHOWAS      =================================

-- Returns a 2 dimensional array, array one contains n arrays with 4 or 6 strings each.
-- array[n] = [nick, user, host, realname]
-- - or -
-- array[n] = [nick, user, host, realname,server,serverinfo]
-- Call: self~WhoWas(nick[,limit, target, waittime])
-- Example:
--  list = self~whowas("can",5)
--  if list = .nil then say "There was no such nick!"
--  else do i = 1 to list~items
--    do string over list[i]
--      call charout ,string' '
--    end
--    Say
--  end


::METHOD WhoWas
    expose whowas_done client
    parse arg nick, limit, target
    whowas_done = .false
    message = .WhoWasResult~new(.message~new(self, "WHOWASCANCEL"), nick limit target) 
    client~write(message)
    client~MessageHandler~__RegisterFunctionCall(message)
    timeoutalarm = .alarm~new(.local['IRC.TIMEOUT.SCRIPTS'], .message~new(self, 'WhoWasCancel'))
    guard off when WhoWas_done \= .false
    timeoutalarm~cancel
--    client~MessageHandler~__RemoveFunctionCall(message)
    return message

::METHOD WhoWasCancel
    expose WhoWas_done
    WhoWas_done = .true

-- ================ WHO         =================================

-- Returns a single-dimensional array with lines of the following structure:
-- "<channel> <user> <host> <server> <nick>
-- ( "H" / "G" > ["*"] [ ( "@" / "+" ) ] :<hopcount> <real name>"
-- h: here, G: Away, *: IRCOp, @: Op in one channel we're in as well, +: Voiced in one channel

::METHOD Who
    expose who_done client
    parse arg nick
    who_done = .false
    message = .WhoResult~new(.message~new(self, "WHOCANCEL"),nick) 
    client~write(message)
    client~MessageHandler~__RegisterFunctionCall(message)
    timeoutalarm = .alarm~new(.local['IRC.TIMEOUT.SCRIPTS'], .message~new(self, 'WhoCancel'))
    guard off when Who_done \= .false
    timeoutalarm~cancel
    client~MessageHandler~__RemoveFunctionCall(message)
    return message

::METHOD WhoCancel
  expose Who_done
  Who_done = .true

/*
 --- Construct for active scripts ---
/* ================             =================================*/
::METHOD X
    expose X_done client
    parse arg 
    X_done = .false
    message = .Result~new(.message~new(self, "XCANCEL"),X) 
    client~write(message)
    client~MessageHandler~__RegisterFunctionCall(message)
    timeoutalarm = .alarm~new(.local['IRC.TIMEOUT.SCRIPTS'], .message~new(self, 'XCancel'))
    guard off when X_done \= .false
    timeoutalarm~cancel
    client~MessageHandler~__RemoveFunctionCall(message)
    return message
::METHOD RPL_X
    expose ResultArray
    use arg message
    do entry over ResultArray
	if entry~class = .XResult then do
	    entry~Result = message
	    entry~Done -- on last message
	    leave
	end
    end
    
::METHOD XCancel
  expose X_done
  X_done = .true
*/






::METHOD Translate
    expose client
    parse arg string
    casemapping = client~settings["CASEMAPPING"]~translate
    SELECT
        when casemapping = "ASCII" then do
	    mapin  = "abcdefghijklmnopqrstuvwxyz"
	    mapout = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	end
	otherwise do
	    mapin  = "abcdefghijklmnopqrstuvwxyz{}|~"
	    mapout = "ABCDEFGHIJKLMNOPQRSTUVWXYZ[]\^"
	end
    END
    return string~translate(mapout,mapin)    