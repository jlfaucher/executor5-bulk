-- The Supported class is a subclass of the directory class.
-- It implements the default values of basically the 005 numrpl
::CLASS IsSupported SUBCLASS directory
::METHOD Init
    expose default
-- The default values here may not be accurate. Thus experience from users is highly recommended!
    default = .directory~new
    default["CASEMAPPING"]	= "rfc1459"
    default["CHANNELLEN"]	= 200
    default["CHANTYPES"]	= "#&"
    default["NICKLEN"]	= 9
    default["PREFIX"]	= "(ov)@+"
    default["CHANTYPES"]	= "#&"
    default["MODES"]	= 3
    default["MAXBANS"]	= 25
    default["TOPICLEN"]	= 80
    default["KICKLEN"]	= 80
    default["MAXTARGETS"]	= 4
    
    -- at the time of the initialization the default values are copied to the current values. They
    -- are expected to be overwritten by the IRC server in the progress of establishing the connection.
    do name over default
	self~SetEntry(name,default[name])
    end

-- This method resets a value to its default value
-- It is only called in case the client receives a message saying to drop the value received earlier for
-- this particular setting.
::METHOD Revert
    expose default
    use arg name
    self~SetEntry(name,default[name])

