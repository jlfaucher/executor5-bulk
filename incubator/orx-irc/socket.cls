-- Name: Socket interface for IRCBOT (now orx-irc)
-- Written by Moritz Hoffmann 2006, part of the orx-irc project
-- Released under the BSD license


-- TODO:
-- -Clean the code up

::CLASS socket_init
::METHOD init CLASS
rc = RxFuncAdd("SockLoadFuncs","rxsock","SockLoadFuncs")
rc = SockLoadFuncs(0)
rc = SockInit()

::CLASS socket PUBLIC
::METHOD Init
  expose host connected
  use arg host
  connected = .false

::METHOD Uninit
  self~disconnect

::METHOD Host UNGUARDED
  expose host
  return host

::METHOD Connect
  expose host socket connected
  if connected = .true then return
  
  -- this method connects to the server
  socket = SockSocket("AF_INET","SOCK_STREAM","0")
  -- Don't change this!
  !temp.!family = "AF_INET" 
  !temp.!port = host~port
  !temp.!addr = host~ip
--  say 'Trying to connect to server, timeout is 75s.'
  ret = SockConnect(socket,"!temp.!")
  if ret \= 0 then return ret
--    say '_Errno:' SockSock_Errno() 'errno' errno 'h_errno' h_errno '!!!!'
--  Say 'Connected to server '||host~ip||' on socket '||socket
--  Say 'Setting IO...'
  ret = Sockioctl(socket,'FIONBIO',1)
  if ret = -1 & errno \= 'EWOULDBLOCK' then do
    Say 'Error Connecting...'
    connected = .false
    return -1
  end
  connected = .true
  return 0

::METHOD connected ATTRIBUTE

-- Disconnects the socket
::METHOD disconnect UNGUARDED
  expose socket connected
  if connected = .true then
    rc = SockClose(socket)
  else rc = 0
  connected = .false
  Return rc

-- Readable: returns true if there's data available to be read.
::METHOD Readable UNGUARDED
  expose socket connected
  -- if connected \= .true then return .false
  r.0=1;r.1=socket
  w.0=1;w.1=socket
  e.0=1;e.1=socket
  rc = SockSelect('r.','w.','e.',10)
  -- ret = charout(,((r.1 = socket)&(rc>0))||' '||rc r.1'='socket' ')
  if rc < 1 then do
    self~disconnect
    return .false
  end
  return (r.1 = socket)&(rc>0)
  
-- reads input from the socket. the length may be given. default is 4096.
::METHOD read
  expose Socket connected readq
  if connected \= .true then return -1
  if datatype(arg(1)) = 'NUM' then
    len = arg(1)
  else len = 4096
  input = ''
  ret = SockRecv(socket,"recvstr",len)
  if ret > 0 then input = left(recvstr,ret)
  if ret = -1 & errno \= 'EWOULDBLOCK' then do
    Say 'Error while reading from socket...' errno
    say SockSock_Errno() sockPSock_Errno() errno h_errno
    self~disconnect
    return ''
  end
  return input

-- writes a string on the socket
::METHOD Write UNGUARDED 
  expose socket connected
  if connected \= .true then return -1
  parse arg StringToSend
  ret = 0
--  say 'Writing "'StringToSend'" on Socket #'Socket
  ret = SockSend(socket,stringtosend)
  if ret = -1 then do
    say SockSock_Errno() sockPSock_Errno() errno h_errno
    if rc < 1 then do
      self~disconnect
      return -1
    end
  end
  return ret

::CLASS lnsocket PUBLIC SUBCLASS socket

-- Method Init accepts 2 parameters: A Host object and a client object.
-- The client object needs to have a public method named AVAILABLE.
-- It's called with the line (string) as parameter.
-- The client also needs to have a DISCONNECT method to indicate disconnects.

::METHOD init
  expose client
  use arg host, client
  forward class (super) array (host)

::METHOD connect
  ret = self~connect:super
  reply ret
  self~readserv

::METHOD Readserv PRIVATE UNGUARDED
    expose client
    input = ''
    do while self~connected
	if self~readable then input = input||self~read
	do while input~pos("0D0A"x) > 0
	    parse var input message "0D0A"x input
	    client~Available(message)
	end
	Call SysSleep .25
    end
    
::CLASS host PUBLIC
::METHOD sethost
  expose ip name port
  use arg iparg
  parse var iparg local.host':'local.port
  parse var local.host num.1'.'num.2'.'num.3'.'num.4
  IsIP = .true
  do i = 1 to 4
    IsIP = IsIP & datatype(num.i,'Number')
  end
-- IsIP will be .true if all of the num elements are numbers. If IsIP is
-- false then we have to look for the IP number. if IsIP is .true the user supplied an
-- IP number. 
  if \ IsIP then do
    -- test if the string was a hostname
--    Say 'Trying to resolve '||local.host||'. This may take some time.'
    rc = SockGetHostByName(local.host,'host.')
    if rc <> 0 then do
--      say 'Resolved '||host.name||' to '||host.addr||'. RC was:'||rc
--      do i = 1 to host.alias.0
--        say 'Aliases: '||host.alias.i
--      end
--      do i = 1 to host.addr.0
--        say 'Addresses: '||host.addr.i
--      end
      ip = host.addr
    end
    else return -1
  end
  else ip = local.host
  if (space(local.port) = '') & \Datatype(local.port,'Number'),
    then port = 6667
    else port = local.port
  name = local.host
return 0

::METHOD ip
  expose ip
  return ip
::METHOD port
  expose port
  return port
::METHOD name
  expose name
  return name