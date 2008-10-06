/*  sfclient.rex  */
/*  David Ruggles   2:03:26 PM  Monday, October 06, 2008  */
/*  This program uses the socket function package
    to create a basic socket client  */

/*  load socket function package if it's not already loaded  */
    if rxfuncquery('SockDropFuncs') then do
        call rxfuncadd 'SockLoadFuncs', 'rxsock', 'SockLoadFuncs'
        call SockLoadFuncs 'bypasscopyright'
    end

/*  create a socket  */
    socket = socksocket('AF_INET', 'SOCK_STREAM', 'IPPROTO_TCP')

/*  specify the host we will connect to  */
    host.!family = 'AF_INET'        --  Protocol family (only AF_INET is supported)
    host.!addr = SockGetHostId()    --  IP address (use the sockgethostid function to get address of the localhost)
    host.!port = '726578'           --  Port number

/*  connect to the server  */
    if sockconnect(socket, 'host.!') < 0 then do
        say 'SockConnect Failed'
        exit
    end

    say 'type "X" to exit'
    do forever
        call charout , 'Send To Server: '
        parse pull message
        if message~upper() = 'X' then leave
    /*  send message to server  */
        if socksend(socket, message) < 0 then do
            say 'SockSend Failed'
            leave
        end
    /*  get message from server  */
        ret = sockrecv(socket, 'data', 1024)
        if ret < 1 then do
            if ret < 0 then
                say 'SockRecv Failed'
            else
                say 'Socket Closed'
            leave
        end
        say 'Server Responded:' data
    end

/*  close the socket connection  */
    if sockclose(socket) < 0 then
        say 'SockClose Failed'