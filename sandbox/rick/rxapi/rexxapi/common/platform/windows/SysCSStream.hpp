
#ifndef SysCSStream_Included
#define SysCSStream_Included


#include "CSStream.hpp"

/**
 * A socket implementation of a CSStream for Windows. This is a base class that reads writes to a socket independently of how the socket is instantiate.
 */
class SysSocketConnection : public ApiConnection
{
public:
    inline SysSocketConnection() : c(-1), ApiConnection() { }
    inline SysSocketConnection(SOCKET sock) : c(sock), ApiConnection() { }
    inline ~SysSocketConnection() { }

    virtual bool read(void *buf, size_t bufsize, size_t *bytesread);
    virtual bool write(void *buf, size_t bufsize, size_t *byteswritten);
    virtual bool write(void *buf, size_t bufsize, void*buf2, size_t buf2size, size_t *byteswritten);

    virtual bool disconnect();

protected:
    SOCKET c; // stream socket
};


/**
 * This is the client stream for a TCP/IP style connection.
 */
class SysInetSocketConnection : public SysSocketConnection
{
public:
    SysInetSocketConnection() : SysSocketConnection() { };
    SysInetSocketConnection(const char *host, int port);
    ~SysInetSocketConnection();

    bool connect(const char *, int);
};


/**
 * Base class for all socket-based server connections. The subclasses
 * provide all of the setup around the socket.
 */
class SysServerSocketConnectionManager : public ServerConnectionManager
{
public:
    inline SysServerSocketConnectionManager() : c(-1) { }
    inline ~SysServerSocketConnectionManager() { }

    virtual bool disconnect();
    virtual ApiConnection *acceptConnection();

protected:
    SOCKET c; // stream socket
};

/**
 * Implementation class for a socket connection bound to localhost.
 */
class SysServerInetSocketConnectionManager : public SysServerSocketConnectionManager
{
public:
    inline SysServerInetSocketConnectionManager() { }
    inline ~SysServerInetSocketConnectionManager() { }

    bool bind(int port);
};
#endif
