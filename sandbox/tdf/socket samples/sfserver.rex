/*  sfserver.rex  */
/*  David Ruggles   2:03:37 PM  Monday, October 06, 2008  */
/*  This program uses the socket function package
    to create a basic socket echo server  */

    srv = .myserver~new()
    srv~listen()

::class myserver

::method init
    expose socket

/*  load socket function package if it's not already loaded  */
    if rxfuncquery('SockDropFuncs') then do
        call rxfuncadd 'SockLoadFuncs', 'rxsock', 'SockLoadFuncs'
        call SockLoadFuncs 'bypasscopyright'
    end

/*  create a socket  */
    socket = socksocket('AF_INET', 'SOCK_STREAM', '0')

::method monitor unguarded
    expose socket

    say 'Press Ctrl-C To Shutdown'
    do forever
        if sysgetkey('noecho')~c2x() = '03' then leave
    end

/*  close the socket  */
    if sockclose(socket) < 0 then
        say 'SockClose Failed'

::method listen
    expose socket

/*  specify the host we will run as  */
    host.!family = 'AF_INET'        --  Protocol family (only AF_INET is supported)
    host.!addr = SockGetHostId()    --  IP address (use the sockgethostid function to get address of the localhost)
    host.!port = '726578'           --  Port number

/*  bind to the host information  */
    if sockbind(socket, 'host.!') < 0 then do
        say 'SockBind Failed'
        exit
    end

/*  start listening for new connections  */
    if socklisten(socket, 256) < 0 then do
        say 'SockListen Failed'
        exit
    end

    self~start('monitor')   --  this will allow the server to be shutdown cleanly

    do forever
        clientsocket = sockaccept(socket)   --  prepare to accept a new client
        if clientsocket = '-1' then leave   --  if the socket is closed (by monitor) sockaccept will fail
    /*  this will spawn a thread to handle the new client and then return to accept the next client  */
        self~start('respond', clientsocket)
    end

::method respond unguarded
    use arg socket

    do forever
        /*  get data from the client  */
        if sockrecv(socket, 'data', 1024) < 1 then leave
        /*  echo that data back to the client  */
        call socksend socket, 'Echo:' data
    end
