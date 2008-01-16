-- Class definition file for message objects
-- Written by Moritz Hoffmann 2006, part of the orx-irc project
-- Released under the BSD license

-- base message class for all messages floating around
-- only offers the TimeStamp method that returns the time the message was
-- created. As messages (normally) get dreated as a result of incoming data
-- the timestamp shuold quite adequat

-- Each message class that can be used in the direction client -> server
-- must have a method IRCString that converts the message into a string
-- representing the message.
-- The method should only be called by the write method of the IRC class.

-- This is the parent class to all other message classes
::CLASS IRCMessage
::METHOD Init
    expose TimeStamp
    TimeStamp = NowToTS()
    -- The following line is needed to initialize mixinclasses of the
    -- ircmessage's subclasses
    forward class (super)
::METHOD TimeStamp
    expose TimeStamp
    return TimeStamp
-- There are a lot of different messages, for thier hirarchy have a look at messages.txt in
-- the doc directory 


-- user messages superclass, includes a sender
::CLASS UMSG SUBCLASS IRCMessage 
::METHOD Init
  expose Sender
  use arg Sender
  self~init:super
::METHOD Sender
  expose Sender
  return Sender

-- This is the message class for sending a password to the server
-- It is only a subclass of the umsg class to maintain the correct tree
-- structure, although it has no explicit sender. The sender is always self.
-- As this message is only created by the IRC class itself this shouldn't
-- bring any problems.

::CLASS msgpass SUBCLASS umsgr
::METHOD Init
    expose pass
    use arg pass
::METHOD IRCString
    expose pass
    return "PASS" pass

-- This is the user message that initiates a connection
-- This class is an instance of the umsg class but has no sender analog
-- to the msgpass class.

::CLASS msguser SUBCLASS umsgr
::METHOD Init
    expose username mode realname
    use arg username, mode, realname
::METHOD Username
    expose username; return username
::METHOD Mode
    expose mode; return mode
::METHOD RealName
    expose realname; return realname
::METHOD IRCString
    return "USER" self~username self~mode "* :"self~realname

-- nick change message, method nick to find out the new nick, string
::CLASS msgnick SUBCLASS umsg 
::METHOD init
    expose newnick
    use arg sender,newnick
    self~init:super(sender)
::METHOD nick
    expose newnick
    return newnick
::METHOD IRCString
    expose newnick
    return "NICK" newnick
::CLASS msgNickResult SUBCLASS msgNick INHERIT Result

-- quit message, includes a string
::CLASS msgquit SUBCLASS umsg 
::METHOD Init
  expose string
  string = ''
  use arg sender,string
  self~init:super(sender)
::METHOD string
  expose string
  return string
::METHOD IRCString
    expose string
    return "QUIT :"string
    
-- superclass for all messages that have a special receiver other than self and a string
::CLASS umsgr SUBCLASS umsg 
::METHOD Init
  expose receiver string
  string = ''
  use arg sender,receiver,string
  self~init:super(sender)
::METHOD receiver
  expose receiver
  return receiver
::METHOD string
  expose string; return string

-- normal text message to either channel or self
::CLASS privmsg SUBCLASS umsgr 
::METHOD IRCString
    return "PRIVMSG "self~receiver~MakeString":"self~string

-- same like privmsg only that notice's are less common, and they don't
-- produce any replies
::CLASS unotice SUBCLASS umsgr 
::METHOD IRCString
    return "NOTICE "self~receiver~MakeString":"self~string

-- invite message, always directed at self, string is the list of channels
::CLASS invite SUBCLASS umsgr
::METHOD IRCString
    return "INVITE" self~receiver self~string
::CLASS inviteResult SUBCLASS invite INHERIT Result

-- kill message, includes a sender, a receiver and a string (the reason)
::CLASS msgkill SUBCLASS umsgr PUBLIC
::METHOD IRCString
    return "KILL" self~receiver":"self~string
::CLASS msgKillResult SUBCLASS msgKill INHERIT Result

-- kick message, sender, receiver, channel, string
::CLASS msgkick SUBCLASS umsgr PUBLIC
::METHOD Init
  expose Channel
  use arg Sender, Receiver, Channel, String
  self~init:super(Sender,Receiver,String)
::METHOD Channel
  expose Channel
  return Channel
::METHOD IRCString
    return "KICK" self~Channel self~Receiver

::CLASS msgKickResult SUBCLASS msgKick INHERIT Result

-- part message, sender, receiver, string
::CLASS msgpart SUBCLASS umsgr PUBLIC
::METHOD IRCString
    return "PART" self~Sender":"self~String
::CLASS msgPartResult SUBCLASS msgPart INHERIT Result

-- topic message, server, receiver, string
::CLASS msgtopic SUBCLASS umsgr PUBLIC
::METHOD IRCString
    return "TOPIC" self~receiver":"self~string
::CLASS msgTopicResult SUBCLASS msgTopic INHERIT Result

-- mode message, sender, receiver, string
::CLASS msgmode SUBCLASS umsgr PUBLIC
::METHOD IRCString
    return "MODE" self~receiver self~string
::CLASS msgModeResult SUBCLASS msgMode INHERIT Result
    
-- join message, sender, receiver, no string
::CLASS msgjoin SUBCLASS umsgr PUBLIC
::METHOD Init
    expose key
  use arg sender, receiver
  if arg() = 3 then key = arg(3)
  else key = ''
  self~init:super(sender,receiver)
::METHOD IRCString
    return "JOIN" self~receiver self~key
::METHOD Key ATTRIBUTE

-- Server messages

-- I'm not sure how the server messages will be handled in future. maybe they are just internal
-- objects that are not passed on to the script object. That would result in a more limited
-- functionality of the program.
-- the other way would be to generate the necessary replies (like ping) in the client calss
-- but still forward the massage to the sripting object. the server messages are normally
-- very important to the client's stability thus it's needed to work on them in the core
-- client. I don't think it would be needed for a user to replace the ping functionality
-- of the client when the message itself gets forwarded.


::CLASS smsg SUBCLASS ircmessage PUBLIC
::METHOD Init
    expose string
    use arg string
    self~init:super
::METHOD String
    expose string
    return string
-- ping class (missing: string that needs to be replied!)
-- the reply in generated by the client due to performance and securety reasons
::CLASS msgPing SUBCLASS smsg PUBLIC
::METHOD IRCString
    return "PING :"self~string
::CLASS msgPingResult SUBCLASS msgPing INHERIT Result

::CLASS msgPong SUBCLASS smsg
::METHOD IRCString
    return "PONG :"self~String


-- error message, leads to a disconnect.
::CLASS msgerror SUBCLASS smsg PUBLIC
::METHOD Init
  self~init:super('') -- error messages don't necessarily have a string connected with them

-- server notice
::CLASS snotice SUBCLASS smsg PUBLIC

-- wallops message, need to test this as it's only copied from the manual...
::CLASS msgwallops SUBCLASS smsg PUBLIC

-- a numeric message, the script object will work on it.
::CLASS nummsg SUBCLASS smsg PUBLIC
::METHOD Init
  expose Number
  use arg Number, String
  self~init:super(String)
::METHOD Number
  expose Number
  return Number

-- a mode message originating from the server
::CLASS smode SUBCLASS smsg PUBLIC
::METHOD Init
  expose Server Target
  use arg Server, Target, String
  self~init:super(String)
  return self

::METHOD Server
  expose Server
  return Server
::METHOD Target
  expose Target
  return Target


