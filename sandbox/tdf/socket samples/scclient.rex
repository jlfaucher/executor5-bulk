/*  scclient.rex  */
/*  David Ruggles   3:02:26 PM  Monday, October 06, 2008  */
/*  This program uses the socket class
    to create a basic socket client  */

/*  instaniate an instance of the socket class  */
    sock = .socket~new()

/*  instaniate an instance of the inetaddress class
    with the host information of the server we will
    contact: localhost and port 726578
    we use the "gethostid" class method of the socket
    class to determine the localhost address  */
    host = .inetaddress~new(.socket~gethostid(), '726578')

/*  connect to the server  */
    if sock~connect(host) < 0 then do
        say 'Connect Failed'
        exit
    end

    say 'type "X" to exit'
    do forever
        call charout , 'Send To Server: '
        parse pull message
        if message~upper() = 'X' then leave
    /*  send message to server  */
        if sock~send(message) < 0 then do
            say 'Send Failed'
            leave
        end
    /*  get message from server  */
        ret = sock~recv(1024)
        if ret = .nil then do
            if sock~errno() < 0 then
                say 'Recv Failed'
            else
                say 'Socket Closed'
            leave
        end
        say 'Server Responded:' ret
    end

/*  close the socket connection  */
    if sock~close() < 0 then
        say 'SockClose Failed'

::requires 'c:\dev\oorexx development\main\trunk\extensions\rxsock\socket.cls'