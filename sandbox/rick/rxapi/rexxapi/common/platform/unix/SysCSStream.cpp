/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2018 Rexx Language Association. All rights reserved.    */
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
#include "oorexxapi.h"
#include "SysCSStream.hpp"
#include "ServiceException.hpp"

/**
 * Read from the connection.
 *
 * @param buf       Target buffer for the read operation.
 * @param bufsize   Size of the target buffer.
 * @param bytesread Number of bytes actually read.
 *
 * @return True on an error, otherwise false
 */
bool SysSocketConnection::read(void *buf, size_t bufsize, size_t *bytesread)
{
    if (c == -1)
    {
        errcode = CSERROR_IO_FAILED;
        return false;
    }
    int actual = recv(c, (char *)buf, (int)bufsize, 0);
    if (actual == -1)
    {
        // a -1 return is a bad problem.  0 might be bad, but allow the
        // caller to handle that one.
        errcode = CSERROR_IO_FAILED;
        return false;
    }
    *bytesread = (size_t)actual;
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
bool SysSocketConnection::write(void *buf, size_t bufsize, size_t *byteswritten)
{
    if (c == -1)
    {
        errcode = CSERROR_IO_FAILED;
        return false;
    }
    int actual = send(c, (char *)buf, (int)bufsize, 0);
    if (actual == -1)
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

    if (c == -1)
    {
        errcode = CSERROR_IO_FAILED;
        return false;
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

    int actual = send(c, buffer, (int)bufferSize, 0);
    // we're done with the buffer, regardless of whether this works or fails
    returnMessageBuffer(buffer);
    if (actual == -1)
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
 * Close the server connection.
 *
 * @return True on an error, otherwise false
 */
bool SysSocketConnection::disconnect()
{
    if (c != -1)
    {
        closesocket(c);
        c = -1;
        errcode = CSERROR_OK;
        return true;
    }
    else
    {
        errcode = CSERROR_INTERNAL;
        return false;
    }
}


/**
 * Alternate constructor.
 *
 * @param host   String name of the host.
 * @param port   Target port number.
 */
SysLocalSocketConnection::SysLocalSocketConnection(const char *serviceName) : SysSocketConnection()
{
    connect(serviceName);
}


/**
 * Open a connection to a named local service
 *
 * @param host   The target service name.
 *
 * @return True on an error, otherwise false.
 */
bool SysLocalSocketConnection::connect(const char *serviceName)
{
    // make sure we're not already connected.
    if (c != -1)
    {
        errcode = CSERROR_ALREADY_CONNECTED;
        return false;
    }

    // get a socket
    c = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (c == -1)
    {
        errcode = CSERROR_INTERNAL;
        return false;
    }

    // bind the server socket to a service name
    struct sockaddr_in name; // address structure
    name.sun_family = AF_LOCAL;
    strncpy (name.sun_path, serviceName, sizeof (name.sun_path));
    // make sure this is null terminated
    name.sun_path[sizeof (name.sun_path) - 1] = '\0';

    size_t size = (offsetof (struct sockaddr_un, sun_path)
          + strlen (name.sun_path));

    if (::connect(c, (struct sockaddr *) &name, size) == -1)
    {
        errcode = CSERROR_OPEN_FAILED;
        closesocket(c);
        c = -1;
        return false;
    }

    errcode = CSERROR_OK;
    return true;
}


/**
 * Accept a connection from a client.
 *
 * @return True on an error, otherwise false
 */
ApiConnection *SysServerSocketConnectionManager::acceptConnection()
{
    struct sockaddr_in addr; // address structure
    int sz = sizeof(addr);

    if (c == -1)
    {
        errcode = CSERROR_INTERNAL;
        return NULL;
    }
    SOCKET client = accept(c, (struct sockaddr *) &addr, &sz);
    if (client == -1)
    {
        errcode = CSERROR_CONNX_FAILED;
        return NULL;
    }

    errcode = CSERROR_OK;
    // now create an object wrapper for this client connection.
    return new SysSocketConnection(client);
}


/**
 * Close the connection to the host.
 *
 * @return True on an error, otherwise false.
 */
bool SysServerSocketConnectionManager::disconnect()
{
    if (c != -1)
    {
        closesocket(c);
        c = -1;
        // clean up service name from the file system before terminating
        unlink(boundServiceName);
        boundServiceName = NULL;
        errcode = CSERROR_OK;
        // this is only done when the server is shutting down prior
        // to termination. We don't really need to get rid of this, but
        // it is good practice
        free((void *)userServiceName);
        userServiceName = NULL;
        return true;
    }
    else
    {
        errcode = CSERROR_UNKNOWN;
        return false;
    }
}


/**
 * Make a server connection.
 *
 * @param port   Port to use for the connection.
 *
 * @return True on an error, otherwise false
 */
bool SysServerLocalSocketConnectionManager::bind(const char *serviceName)
{

    // get a server socket
    c = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (c == -1)
    {
        errcode = CSERROR_UNKNOWN;
        return false;
    }
    // bind the server socket to a service name
    struct sockaddr_in name; // address structure
    name.sun_family = AF_LOCAL;
    strncpy (name.sun_path, serviceName, sizeof (name.sun_path));
    // make sure this is null terminated
    name.sun_path[sizeof (name.sun_path) - 1] = '\0';

    size_t size = (offsetof (struct sockaddr_un, sun_path)
          + strlen (name.sun_path));

    // do the bind operation
    if (::bind(c, (struct sockaddr *) &name, size) == -1)
    {
        errcode = CSERROR_CONNX_EXISTS;
        return false;
    }
    // listen for a client at the port
    if (listen(c, 20) == -1)
    {
        errcode = CSERROR_INTERNAL;
        return false;
    }

    // remember the service name we are bound to.
    boundServiceName = serviceName;
    errcode = CSERROR_OK;
    return true;
}


/**
 * Check for the availability of the file name used for this bound service,
 * deleting stale versions if still there.
 *
 * @param serviceName
 *               The target file path
 *
 * @return true if the file name is available for use, false otherwise.
 */
bool SysServerLocalSocketConnectionManager::checkServiceName(const char *serviceName)
{
    struct stat st;
    int status = stat(serviceName, &st);
    if (status == 0)
    {
       // A file already exists. Check if this file is a socket node.
       //   * If yes: unlink it.
       //   * If no: treat it as an error condition.
       //
       if ((st.st_mode & S_IFMT) == S_IFSOCK)
       {
          status = unlink(serviceName);
          if (status != 0)
          {
              return false;
          }
          // unlinked the file, we're good
          return true;
       }
       else
       {
           // we won't unlink a real file...we can't use this name.
           return false;
       }
    }
    else
    {
       if (errno == ENOENT)
       {
           // no file with our intended name, this works
           return true;
       }
       else
       {
           // some other error, should not happen
           return false;
       }
    }
}


/**
 * Generate a unique string to be used for interprocess communications for this userid.
 *
 * @return A unique identifier used to create the named pipes.
 */
const char *SysServerLocalSocketConnectionManager::generatePipeName()
{
    // if we've already generated this, we're done
    if (userServiceName != NULL)
    {
        return userServiceName;
    }

    // a buffer for generating the name
    char pipeNameBuffer[256];

    // we create the file in the user's home path as a hidden file
    const char *homePath
    if ( (homePath = getenv("HOME")) == NULL)
    {
        homepath = getpwuid(getuid())->pw_dir;
    }

    snprintf(pipeNameBuffer, sizeof(pipeNameBuffer), "%s/ooRexx-%d.%d.%d-%s.service", homePath, ORX_VER, ORX_REL, ORX_MOD,
#ifdef __REXX64__
	    "64");
#else
		"32");
#endif

    userServiceName = strdup(pipeNameBuffer);
    return userServiceName;
}
