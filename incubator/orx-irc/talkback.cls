-- Talkback object definition
-- Written by Moritz Hoffmann 2006, part of the orx-irc project
-- Released under the BSD license

--  The purpose of this class is to provide a way to talk to the IRC server.

-- this is the super class for all TakBack classes
::CLASS ClientTB PUBLIC
::METHOD Init
    expose client NumHandler
    use arg client, NumHandler
    -- raise some error when client~class \= .irc ...
    return self
::METHOD Join
    expose client
    parse arg channelname
    if (channelname~right(1)~verify(client~Settings~IsSupport~ChanTypes) = 0) && (channelname~length <= client~Settings~IsSupport~ChannelLen) then do
        client~writeln("JOIN" channelname)
	return .true
    end
    else return .false
	
::METHOD Part
    expose client
    parse arg channelname
    if client~GetReceiverObject(channelname)~class = .channel then do
        client~writeln("PART" channelname)
	return .true
    end
    else return .false
    
::METHOD IsOn
    expose NumHandler
    return NumHandler~IsOn(arg(1))

::METHOD Time
    expose NumHandler
    return NumHandler~Time(arg(1))