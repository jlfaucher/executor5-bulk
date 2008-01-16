#!/opt/ooRexx/bin/rexx
/* */
.debug~initialize(.stream~new("irc.dmp"),"11111"b)

client = .irc~new()

client~GetEventHandler~EventSetMethods(.METHODS)
t = .TestReceiver~new
client~GetEventHandler~EventSetMethod("onConnect",t)
client~connect(.host~new~~sethost('localhost'),.nil,'Antiguru','Cant','A testclient')
--client~connect(.host~new~~sethost('irc.ca.webbnet.info'),.nil,'Can','Cant','A testclient')
--client~connect(.host~new~~sethost('irc.netlabs.org'),.nil,'Can','Cant','A testclient')
if \ client~connected then Say "Error connecting!"
  do while (chars()=0) & (client~connected)
--    call charout ,"."
    Call SysSleep .5
  end
client~disconnect

if (chars()>0) then call Linein

::REQUIRES "irc.cls"


::METHOD OnInit
    expose client
    use arg client

::METHOD OnConnect UNGUARDED
    expose client
    reply
    numeric digits 10
    msg = client~Command~Join("#bc")
    say "join Successful:" msg~Success
--    say "Delay:" msg~Completed msg~TimeStamp
--    message = client~Command~IsOn("Antiguru Can Test")
--    say "Result:"message~result
--    do entry over client~Settings~IsSupported
--	say entry":"client~Settings~IsSupported[entry]
--    end

::METHOD RPL_EndOfMOTD UNGUARDED
    expose client
    say "Current host:" client~Settings~Host
    say "Time created:" client~Settings~Created
    return
    timeo =  client~command~time("irc.localhost")
    say timeo~time timeo~server
    msg = client~command~WhoIs("Antiguru")
    do value over msg~Result
	say value":"msg~Result[value]
    end
    return
--    reply
--    time = client~talkback~time("irc.localhost")
--    do i over time
--	say i
--    end
--    say client~talkback~IsOn("can")


::METHOD OnWrite
    expose client
    say "OnWrite:"||arg(1)~IRCString
--    client~GetEventHandler~EventsetMethod("OnWrite",.Methods~"OnWrite2")


::METHOD OnWrite2
    expose client
    say "OnWrite2:"||arg(1)~IRCString
    client~GetEventHandler~EventRemoveMethod("OnWrite")


::METHOD OnSNotice
    use arg message
    say "OnSNotice:"||message

::METHOD RPL_MOTD
    parse arg string
    say "RPL_MODT:"||string

::METHOD OnChannelMode
    use arg message
    say message~receiver~GetMode
    do user over message~receiver~Users
	say user~MakeString message~receiver~GetMode(user)
    end

::METHOD OnKick
    expose client
    use arg message
    if message~receiver = client~settings~self then do
	client~write(.msgjoin~new(client~settings~self,message~channel))
    end

::METHOD OnSay
    expose client
    use arg message
    say message~sender~MakeString message~string
--    do channel over client~channels
--	say channel~MakeString channel~id
--    end
::METHOD OnInvite
    expose client
    use arg message
    reply
    client~command~join(message~string)

::CLASS TestReceiver
::METHOD OnConnect
    use arg method
    say "Hello from Testreceiver~OnConnect!"
    return 1