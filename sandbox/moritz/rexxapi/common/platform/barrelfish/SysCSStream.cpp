/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2014 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>

#if defined( HAVE_STRINGS_H )
# include <strings.h>
#endif
#include <errno.h>
#include "SysCSStream.hpp"
#include "ServiceException.hpp"

#include "rexxapi_bindings.h"

/**
 * Write a multi-buffer message to the connection.
 *
 * @param buf     Source buffer for the write operation.
 * @param bufsize Size of the source buffer.
 * @param byteswritten
 *                Number of bytes actually written to the connection.
 *
 * @return True on an error, otherwise false
 */
bool SysSocketConnection::write(void *buf, size_t bufsize, void *buf2, size_t buf2size, size_t *byteswritten)
{
    // if the second buffer is of zero size, we can handle without
    // copying
    if (buf2size == 0)
    {
        return write(buf, bufsize, byteswritten);
    }

    size_t bufferSize = bufsize + buf2size;

    // get a buffer large enough for both buffer
    char *buffer = getMessageBuffer(bufferSize);
    // if we can't get a buffer, then try sending this in pieces
    if (buffer == NULL)
    {
        // write the first buffer
        if (!write(buf, bufsize, byteswritten))
        {
            return false;
        }
        size_t buf2written = 0;
        if (!write(buf2, buf2size, &buf2written))
        {
            return false;
        }
        *byteswritten += buf2written;
        return true;
    }

    // copy the message and attached data into a single buffer
    memcpy(buffer, buf, bufsize);
    memcpy(buffer + bufsize, buf2, buf2size);

    size_t actual;
    int ok = write((void *) buffer, bufferSize, &actual);
    // we're done with the buffer, regardless of whether this works or fails
    returnMessageBuffer(buffer);
    if (!ok)
    {
        // a -1 return is a bad problem.  0 might be bad, but allow the
        // caller to handle that one.
        errcode = CSERROR_IO_FAILED;
        return false;
    }
    *byteswritten = (size_t)actual;
    errcode = CSERROR_OK;
    return true;
}

/**
 * Get a buffer for sending a buffered message.
 *
 * @param size   The required size.
 *
 * @return A pointer to a buffer, or NULL if unable to allocate.
 */
char *SysSocketConnection::getMessageBuffer(size_t size)
{
    // if larger than our cached buffer, return
    if (size > MAX_CACHED_BUFFER)
    {
        char *buffer = (char *)malloc(size);
        if (buffer == NULL)
        {
            throw new ServiceException(SERVER_FAILURE, "Error allocating message buffer");
        }
        return buffer;
    }
    // use our cached buffer, allocating it if required.
    if (messageBuffer == NULL)
    {
        messageBuffer = (char *)malloc(MAX_CACHED_BUFFER);
        if (messageBuffer == NULL)
        {
            throw new ServiceException(SERVER_FAILURE, "Error allocating message buffer");
        }
    }
    return messageBuffer;
}


/**
 * Return a message buffer after sending a message.  This will
 * either cache the buffer, or release it, depending upon
 * how it was obtained in the first place.
 *
 * @param buffer The buffer to release.
 */
void SysSocketConnection::returnMessageBuffer(void *buffer)
{
    if (buffer != messageBuffer)
    {
        free(buffer);
    }
}


/**
 * Standard constructor.
 */
SysClientStream::SysClientStream() : SysSocketConnection()
{
    domain = AF_INET;
    type = SOCK_STREAM;
    protocol = 0;
}


/**
 * Alternate constructor.
 *
 * @param name   Hostname and port in the form "hostname:port".
 */
SysClientStream::SysClientStream(const char *name) : SysSocketConnection()
{
    domain = AF_INET;
    type = SOCK_STREAM;
    protocol = 0;
    open(name);
}

/**
 * Standard destructor.
 */
SysClientStream::~SysClientStream()
{
    if (connection != NULL) {
        close();
    }
}


/**
 * Open a connection to a host/port.
 *
 * @param name   Hostname and port in the form "hostname:port".
 *
 * @return True on an error, otherwise false.
 */
bool SysClientStream::open(const char *name)
{
    // copy the host name so we can separate the host and port values.
    char *hostname = strdup(name);

    char *portstr = strstr(hostname, ":");
    if (portstr == NULL)
    {
        free(hostname);
        errcode = CSERROR_HOSTNAME_PORT;
        return false;
    }
    // split the two pieces with a null terminator.
    *portstr = '\0';
    portstr++;
    int port = atoi(portstr);
    if (port == 0)
    {
        free(hostname);
        errcode = CSERROR_HOSTNAME_PORT;
        return false;
    }
    // do the open and free the temp name value.
    bool result = open(hostname, port);
    free(hostname);
    return result;
}


/**
 * Open a connection to a host/port.
 *
 * @param host   The target host name.
 * @param port   The connection port number.
 *
 * @return True on an error, otherwise false.
 */
bool SysClientStream::open(const char *host, int port)
{
    connection = rexx_connect();
    return true;
}


/**
 * Close the connection to the host.
 *
 * @return True on an error, otherwise false.
 */
bool SysClientStream::close()
{
    rexx_disconnect(connection);
    return true;
}

/**
 * Read from the connection.
 *
 * @param buf       Target buffer for the read operation.
 * @param bufsize   Size of the target buffer.
 * @param bytesread Number of bytes actually read.
 *
 * @return True on an error, otherwise false
 */
bool SysClientStream::read(void *buf, size_t bufsize, size_t *bytesread)
{
    rexx_read(connection, (uint8_t *) buf, bufsize, bytesread);
    errcode = CSERROR_OK;
    return true;
}


/**
 * Write a buffer to the connection.
 *
 * @param buf     Source buffer for the write operation.
 * @param bufsize Size of the source buffer.
 * @param byteswritten
 *                Number of bytes actually written to the connection.
 *
 * @return True on an error, otherwise false
 */
bool SysClientStream::write(void *buf, size_t bufsize, size_t *byteswritten)
{
    rexx_write(connection, (uint8_t *) buf, bufsize, byteswritten);
    errcode = CSERROR_OK;
    return true;
}


/**
 * Standard constructor.
 */
SysServerStream::SysServerStream()
{
    errcode = CSERROR_OK;

    server = rexx_export();
}


/**
 * Standard destructor.
 */
SysServerStream::~SysServerStream()
{
    rexx_close(server);
}


/**
 * Accept a connection from a client.
 *
 * @return True on an error, otherwise false
 */
SysServerConnection *SysServerStream::connect()
{
    struct rexx_client *client = rexx_accept(server);
    return new SysServerConnection(this, client);
}


/**
 * Close the server connection.
 *
 * @return True on an error, otherwise false
 */
bool SysServerStream::close()
{
    if (s != -1)
    {
        ::close(s);
        s = -1;
    }
    else
    {
        errcode = CSERROR_INTERNAL;
        return false;
    }
    errcode = CSERROR_OK;
    return true;
}


/**
 * Server connection constructor.
 *
 * @param s      The parent server connection.
 * @param socket The socket for the connection.
 */
SysServerConnection::SysServerConnection(SysServerStream *s, struct rexx_client *client) : SysSocketConnection()
{
	this->server = s;
    this->client = client;
}

/**
 * Standard destructor.
 */
SysServerConnection::~SysServerConnection()
{
    disconnect();
}


/**
 * Is the connection with the localhost?
 *
 *
 * @return True if the client is at address 127.0.0.1, otherwise false
 */
bool SysServerConnection::isLocalConnection()
{
    return true;
}


/**
 * Close the connection to the host.
 *
 * @return True on an error, otherwise false.
 */
bool SysServerConnection::disconnect()
{
    return true;
}

/**
 * Read from the connection.
 *
 * @param buf       Target buffer for the read operation.
 * @param bufsize   Size of the target buffer.
 * @param bytesread Number of bytes actually read.
 *
 * @return True on an error, otherwise false
 */
bool SysServerConnection::read(void *buf, size_t bufsize, size_t *bytesread)
{
    rexx_server_read(client, (uint8_t *) buf, bufsize, bytesread);
    errcode = CSERROR_OK;
    return true;
}


/**
 * Write a buffer to the connection.
 *
 * @param buf     Source buffer for the write operation.
 * @param bufsize Size of the source buffer.
 * @param byteswritten
 *                Number of bytes actually written to the connection.
 *
 * @return True on an error, otherwise false
 */
bool SysServerConnection::write(void *buf, size_t bufsize, size_t *byteswritten)
{
    rexx_server_write(client, (uint8_t *) buf, bufsize, byteswritten);
    errcode = CSERROR_OK;
    return true;
}

