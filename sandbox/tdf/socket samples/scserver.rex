/*  scserver.rex  */
/*  David Ruggles   3:07:47 PM  Monday, October 06, 2008  */
/*  This program uses the socket class
    to create a basic socket echo server  */

srv = .myserver~new()
srv~listen()

::requires 'socket.cls'

::class myserver

::method init
    expose sock

/*  instaniate an instance of the socket class  */
    sock = .socket~new()

::method monitor unguarded
    expose sock

    say 'Press Ctrl-C To Shutdown'
    do forever
        if sysgetkey('noecho')~c2x() = '03' then leave
    end

/*  close the socket  */
    if sock~close() < 0 then
        say 'SockClose Failed'

::method listen
    expose sock

/*  instaniate an instance of the inetaddress class
    with the host information of the server we will
    contact: localhost and port 726578
    we use the "gethostid" class method of the socket
    class to determine the localhost address  */
    host = .inetaddress~new(.socket~gethostid(), '726578')

/*  bind to the host information  */
    if sock~bind(host) < 0 then do
        say 'Bind Failed'
        exit
    end

    if sock~listen(256) < 0 then do
        say 'Listen Failed'
        exit
    end

    self~start('monitor')   --  this will allow the server to be shutdown cleanly

    do forever
        csock = sock~accept()   --  prepare to accept a new client
        if .nil = csock then leave  --  if the socket is closed (by monitor) sockaccept will return .nil
    /*  this will spawn a thread to handle the new client and then return to accept the next client  */
        self~start('respond', csock)
    end

::method respond unguarded
    use arg sock

    do forever
        /*  get data from the client  */
        data = sock~recv(1024)
        if data = .nil then leave
        /*  echo that data back to the client  */
        sock~send('Echo:' data)
    end
