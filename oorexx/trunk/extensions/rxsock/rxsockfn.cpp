/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/***************************************************************************/
/* REXX sockets function support                               rxsockfn.c  */
/*                 sockets utility function package                        */
/***************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <setjmp.h>

/*------------------------------------------------------------------
 * rexx includes
 *------------------------------------------------------------------*/
#include "oorexx.h"
/*------------------------------------------------------------------
 * tcp/ip includes
 *------------------------------------------------------------------*/
#include <sys/types.h>
#include <errno.h>

#if !defined(WIN32)
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/ioctl.h>
    #include <sys/time.h>

    #if defined(OPSYS_AIX) || defined(OPSYS_LINUX)
        #include <unistd.h>
    #endif
    #include <sys/select.h>
    #if defined(OPSYS_SUN)
        #include <sys/filio.h>
    #endif
#endif

#if defined(WIN32)                     // define errno equivalents for windows
    #define sock_errno() WSAGetLastError()
    #define psock_errno(s) fprintf(stderr, "RxSOCK Error: %s\n", s)
#endif

#if defined(OPSYS_AIX) || defined(OPSYS_LINUX)
    #define sock_errno() errno
    #define psock_errno(s) printf("\nSocket error %s\n",s)
#endif

/*------------------------------------------------------------------
 * include for this app
 *------------------------------------------------------------------*/
#include "rxsock.h"

/*------------------------------------------------------------------
 * sock_errno()
 *------------------------------------------------------------------*/
RexxFunction0(int, SockSock_Errno)
{
    return sock_errno();
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * psock_errno()
 *------------------------------------------------------------------*/
RexxFunction1(CSTRING, SockPSock_Errno, OPTIONAL_CSTRING, type)
{
    psock_errno(type);
    return "";
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * accept()
 *------------------------------------------------------------------*/
RexxFunction2(int, SockAccept, int, sock, OPTIONAL_RexxStemObject, stem)
{
    sockaddr_in  addr;
    socklen_t    nameLen;

    nameLen = sizeof(addr);
    int rc = accept(sock, (struct sockaddr *)&addr, &nameLen);

    /*---------------------------------------------------------------
     * set addr, if asked for
     *---------------------------------------------------------------*/
    if (stem != NULLOBJECT)
    {
        sockaddr2stem(context, &addr, stem);
    }
    return rc;
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * bind()
 *------------------------------------------------------------------*/
RexxFunction2(int, SockBind, int, sock, RexxStemObject, stem)
{
    sockaddr_in  addr;

    /*---------------------------------------------------------------
     * get addr
     *---------------------------------------------------------------*/
    stem2sockaddr(context, stem, &addr);

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    return bind(sock, (struct sockaddr *)&addr, sizeof(addr));
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * connect()
 *------------------------------------------------------------------*/
RexxFunction2(int, SockConnect, int, sock, RexxStemObject, stem)
{
    sockaddr_in  addr;

    /*---------------------------------------------------------------
     * get addr
     *---------------------------------------------------------------*/
    stem2sockaddr(context, stem, &addr);

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    return connect(sock,(struct sockaddr *)&addr, sizeof(addr));
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * gethostbyaddr()
 *------------------------------------------------------------------*/
RexxFunction3(int, SockGetHostByAddr, CSTRING, addrArg, RexxStemObject, stem, OPTIONAL_INT, domain)
{
    struct hostent *pHostEnt;
    unsigned long   addr = inet_addr(addrArg);

    if (argumentOmitted(3))
    {
        domain = AF_INET;
    }

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    pHostEnt = gethostbyaddr((char*)&addr, sizeof(addr), domain);

    if (!pHostEnt)
    {
        return 0
    }
    else
    {
        hostent2stem(context, pHostEnt, stem);
        return 1;
    }
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 *  gethostbyname()
 *------------------------------------------------------------------*/
RexxFunction2(int, SockGetHostByName, CSTRING, name, RexxStemObject, stem)
{
    struct hostent *pHostEnt;

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    pHostEnt = gethostbyname(name);

    if (!pHostEnt)
    {
        return 0;
    }
    else
    {
        hostent2stem(context, pHostEnt, stem);
        return 1;
    }
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 *  gethostid()
 *------------------------------------------------------------------*/
RexxFunction0(RexxStringObject, SockGetHostId)
{
    in_addr ia;
#ifdef WIN32
    char     pszBuff[64];                    // buffer for ip address
    PHOSTENT pHostEnt;                       // ptr to hostent structure
    /*
     *   Retrieve my ip address.  Assuming the hosts file in
     *   in %systemroot%/system/drivers/etc/hosts contains my computer name.
     */                                      //get our name
    if (gethostname(pszBuff, sizeof(pszBuff)))
    {
        return context->NewStringFromAsciiz("0.0.0.0");
    }
    pHostEnt = gethostbyname(pszBuff);       // get our ip address
    if (!pHostEnt)
    {
        return context->NewStringFromAsciiz("0.0.0.0");
    }
    ia.s_addr = (*(unsigned long *)pHostEnt->h_addr);// in network byte order already
    return context->NewStringFromAsciiz(inet_ntoa(ia));
#else
#if defined(OPSYS_AIX) || defined(OPSYS_LINUX)
#define h_addr h_addr_list[0]

    char     pszBuff[64];                    /* buffer for ip address*/
    struct hostent * pHostEnt;               /* ptr to hostent structure*/

    /*get our name*/
    if (gethostname(pszBuff, sizeof(pszBuff)))
    {
        return context->NewStringFromAsciiz("0.0.0.0");
    }
    pHostEnt = gethostbyname(pszBuff);     /* get our ip address */
    if (!pHostEnt)
    {
        return context->NewStringFromAsciiz("0.0.0.0");
    }
    ia.s_addr = (*(unsigned long *)pHostEnt->h_addr);// in network byte order already
    return context->NewStringFromAsciiz(inet_ntoa(ia));
#else
    ia.s_addr = htonl(gethostid());
    return context->NewStringFromAsciiz(inet_ntoa(ia));
#endif
#endif
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * getpeername()
 *------------------------------------------------------------------*/
RexxFunction2(int, SockGetPeerName, int, sock, RexxStemObject, stem)
{
    sockaddr_in  addr;
    int          rc;
    socklen_t    nameLen;

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    nameLen = sizeof(addr);
    rc = getpeername(sock,(struct sockaddr *)&addr,&nameLen);

    /*---------------------------------------------------------------
     * write address to stem
     *---------------------------------------------------------------*/
    sockaddr2stem(context, &addr, stem);

    /*---------------------------------------------------------------
     * set return code
     *---------------------------------------------------------------*/
    return rc;
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 *  getsockname()
 *------------------------------------------------------------------*/
RexxFunction2(int, SockGetSockName, int, sock, RexxStemObject, stem)
{
    sockaddr_in  addr;
    int          rc;
    socklen_t    nameLen;

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    nameLen = sizeof(addr);
    rc = getsockname(sock,(struct sockaddr *)&addr,&nameLen);

    /*---------------------------------------------------------------
     * write address to stem
     *---------------------------------------------------------------*/
    sockaddr2stem(&context, addr, sock);

    /*---------------------------------------------------------------
     * set return code
     *---------------------------------------------------------------*/
    return rc;
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 *  getsockopt()
 *------------------------------------------------------------------*/
RexxFunction4(int, SockGetSockOpt, int, sock, CSTRING, level, CSTRING, option, CSTRING, var)
{
    int            rc;
    int            opt;
    struct linger  lingStruct;
    socklen_t      len;
    void          *ptr;
    char           pBuffer[30];


    if (stricmp("SOL_SOCKET", level) != 0)
    {
        context->InvalidRoutine();
        return 0;
    }

    /*---------------------------------------------------------------
     * get option name
     *---------------------------------------------------------------*/
    opt = rxs2SockOpt(option);

    /*---------------------------------------------------------------
     * set up buffer
     *---------------------------------------------------------------*/
    long longVal = 0;
    int intValu = 0;

    switch (opt)
    {
        case SO_LINGER:
            ptr = &lingStruct;
            len = sizeof(lingStruct);
            break;

        case SO_RCVBUF:
        case SO_SNDBUF:
            ptr = &longVal;
            len = sizeof(long);
            break;

        default:
            ptr = &intVal;
            len = sizeof(int);
    }

    /*---------------------------------------------------------------
     * make call
     *---------------------------------------------------------------*/
    rc = getsockopt(sock,SOL_SOCKET,opt,ptr,&len);

    /*---------------------------------------------------------------
     * set return value
     *---------------------------------------------------------------*/
    switch (opt)
    {
        case SO_LINGER:
            sprintf(pBuffer,"%ld %ld", lingStruct.l_onoff, lingStruct.l_linger);
            break;

        case SO_TYPE:
            switch (intVal)
            {
                case SOCK_STREAM: strcpy(pBuffer,"STREAM"); break;
                case SOCK_DGRAM:  strcpy(pBuffer,"DGRAM");  break;
                case SOCK_RAW:    strcpy(pBuffer,"RAW");    break;
                default:          strcpy(pBuffer,"UNKNOWN");
            }
            break;

        case SO_RCVBUF:
        case SO_SNDBUF:
            sprintf(pBuffer, "%ld", longVal);
            break;

        default:
            sprintf(pBuffer,"%ld", intVal);
    }

    // set the variable
    context->SetContextVariable(var, context->NewStringFromAsciiz(buffer));
    return rc;
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 *  ioctl()
 *------------------------------------------------------------------*/
RexxFunction3(int, SockIoctl, int, sock, CSTRING, command, CSTRING, var)
{
    int        cmd = 0;
    void      *data;
    int        dataBuff;
    int        len;
    int        rc;
    char       pBuffer[20];



    if (!stricmp(command, "FIONBIO"))
    {
        cmd = FIONBIO;
        RexxWholeNumber temp;

        if (!context->ObjectToWholeNumber(option, &temp))
        {
            context->InvalidRoutine();
            return 0;
        }
        dataBuff = (int)temp;
        data     = &dataBuff;
        len      = sizeof(int);
    }
    else if (!stricmp(command, "FIONREAD"))
    {
        cmd  = FIONREAD;
        data = &dataBuff;
        len  = sizeof(dataBuff);
    }
    else
    {
        return -1;
    }

    /*---------------------------------------------------------------
     * make call
     *---------------------------------------------------------------*/
#ifdef WIN32
    rc = ioctlsocket(sock,cmd,data);
#else
    rc = ioctl(sock,cmd,data,len);
#endif

    /*---------------------------------------------------------------
     * set output for FIONREAD
     *---------------------------------------------------------------*/
    if (cmd == FIONREAD)
    {
        context->SetContextVariable(var, context->NumberToObject(dataBuff));
    }

    return rc;
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 *  listen()
 *------------------------------------------------------------------*/
RexxFunction2(int, SockListen, int, sock, int, backlog)
{
    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    return listen(sock, backlog);
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 *  recv()
 *------------------------------------------------------------------*/
RexxFunction4(int, SockRecv, int, sock, CSTRING, var, int, dataLen, OPTIONAL_CSTRING, flagVal)
{
    int       flags;
    long      rc;
    char     *pBuffer;
    int       chk;

    /*---------------------------------------------------------------
     * get flags
     *---------------------------------------------------------------*/
    flags = 0;
    if (flagVal != NULL)
    {
        PSZ pszWord;

        pszWord = strtok(flagVal, " ");
        while (pszWord)
        {
            if (!stricmp(pszWord,"MSG_OOB"))  flags |= MSG_OOB;
            else if (!stricmp(pszWord,"MSG_PEEK")) flags |= MSG_PEEK;
            pszWord = strtok(NULL," ");
        }
    }

    /*---------------------------------------------------------------
     * allocate memory for data
     *---------------------------------------------------------------*/
    pBuffer = (char *)malloc(dataLen);
    if (!pBuffer)
    {
        context->InvalidRoutine();
        return 0;
    }

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    rc = recv(sock, pBuffer, dataLen, flags);

    if (-1 == rc)
    {
        dataLen = 0;
    }
    else
    {
        dataLen = rc;
    }

    context->SetContextVariable(var, context->NewString(pBuffer, dataLen));

    free(pBuffer);

    /*---------------------------------------------------------------
     * set return code
     *---------------------------------------------------------------*/
    return rc;
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 *  recvfrom()
 *------------------------------------------------------------------*/
RexxFunction5(int, SockRecvFrom, int, sock, CSTRING, var, int, dataLen, CSTRING, flagArg, OPTIONAL_RexxStemObject, stem)
{
    int       flags;
    long      rc;
    char     *pBuffer;
    int       chk;
    sockaddr_in addr;
    socklen_t   addr_size;


    /*---------------------------------------------------------------
     * get flags
     *---------------------------------------------------------------*/
    flags = 0;
    // if we have a 5th argument, then the 4th argument is a flag value
    if (stem != NULL)
    {
        char *pszWord;

        pszWord = strtok(flagArg, " ");
        while (pszWord)
        {
            if (!stricmp(pszWord,"MSG_OOB"))
            {
                flags |= MSG_OOB;
            }
            else if (!stricmp(pszWord,"MSG_PEEK"))
            {
                flags |= MSG_PEEK;
            }
            pszWord = strtok(NULL," ");
        }
    }
    else
    {
        // the 4th argument is a stem variable
        stem = context->ResolveStemVariable(flagArg);
    }

    stem2sockaddr(stem, &addr);
    addr_size=sizeof(addr);

    /*---------------------------------------------------------------
     * allocate memory for data
     *---------------------------------------------------------------*/
    pBuffer = (char *)malloc(dataLen);
    if (!pBuffer)
    {
        context->InvalidRoutine();
        return 0;
    }

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    rc = recvfrom(sock,pBuffer,dataLen,flags,(struct sockaddr *)&addr,&addr_size);

    if (-1 == rc)
    {
        dataLen = 0;
    }
    else
    {
        dataLen = rc;
    }

    sockaddr2stem(context, &addr, stem);

    context->SetContextVariable(var, context->NewString(pBuffer, dataLen));

    free(pBuffer);

    /*---------------------------------------------------------------
     * set return code
     *---------------------------------------------------------------*/
    return rc;
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/
/*------------------------------------------------------------------
 *  select()
 *------------------------------------------------------------------*/
RexxFunction4(int, SockSelect, OPTIONAL_RexxStemObject, array1, OPTIONAL_RexxStemObject, array2, OPTIONAL_RexxStemObject, array3, OPTIONAL_int, timeout)
{
#if defined(OPSYS_LINUX)
    struct timespec  timeOutS;
    struct timespec *timeOutP;
#else
    struct timeval  timeOutS;
    struct timeval *timeOutP;
#endif
    int             rCount;
    int             wCount;
    int             eCount;
    int            *rArray;
    int            *wArray;
    int            *eArray;
    int             i;
    int             j;
    int             rc;
#if defined(OPSYS_LINUX)
    fd_set   rSetS, *rSet = &rSetS;
    fd_set   wSetS, *wSet = &wSetS;
    fd_set   eSetS, *eSet = &eSetS;
#else
    struct fd_set   rSetS, *rSet = &rSetS;
    struct fd_set   wSetS, *wSet = &wSetS;
    struct fd_set   eSetS, *eSet = &eSetS;
#endif
    int             max;

    /*---------------------------------------------------------------
     * get timeout value
     *---------------------------------------------------------------*/
    if (argumentOmitted(3))
    {
        timeOutP = NULL;
    }
    else
    {
        if (timeout < 0)
        {
            timeout = 0;
        }

        timeOutS.tv_sec  = timeout;
#if defined(OPSYS_LINUX)
        timeOutS.tv_nsec = 0;
#else
        timeOutS.tv_usec = 0;
#endif
        timeOutP = &timeOutS;
    }

    /*---------------------------------------------------------------
     * get arrays of sockets
     *---------------------------------------------------------------*/
    rxstem2intarray(context, array1, &rCount, &rArray);
    rxstem2intarray(context, array2, &wCount, &wArray);
    rxstem2intarray(context, array2, &eCount, &eArray);

/*------------------------------------------------------------------
 * unix-specific stuff
 *------------------------------------------------------------------*/
    /*---------------------------------------------------------------
     * fill in fd_set's
     *---------------------------------------------------------------*/
    FD_ZERO(rSet);
    FD_ZERO(wSet);
    FD_ZERO(eSet);

    for (i=0; i<rCount; i++)
    {
        FD_SET(rArray[i],rSet);
    }
    for (i=0; i<wCount; i++)
    {
        FD_SET(wArray[i],wSet);
    }
    for (i=0; i<eCount; i++)
    {
        FD_SET(eArray[i],eSet);
    }

    /*---------------------------------------------------------------
     * get max number
     *---------------------------------------------------------------*/
    max = 0;
    for (i=0; i<rCount; i++)
    {
        if (rArray[i] > max)
        {
            max = rArray[i];
        }
    }

    for (i=0; i<wCount; i++)
    {
        if (wArray[i] > max)
        {
            max = wArray[i];
        }
    }

    for (i=0; i<eCount; i++)
    {
        if (eArray[i] > max)
        {
            max = eArray[i];
        }
    }

        /*---------------------------------------------------------------
         * make the call
         *---------------------------------------------------------------*/
    rc = select(max+1,rSet,wSet,eSet,timeOutP);

    /*---------------------------------------------------------------
     * fix up the socket arrays
     *---------------------------------------------------------------*/
    if (rc != 0)
    {
        j = 0;
        for (i=0; i<rCount; i++)
        {
            if (FD_ISSET(rArray[i],rSet))
            {
                rArray[j] = rArray[i];
                j++;
            }
        }
        rCount = j;

        j = 0;
        for (i=0; i<wCount; i++)
        {
            if (FD_ISSET(wArray[i],wSet))
            {
                wArray[j] = wArray[i];
                j++;
            }
        }
        wCount = j;

        j = 0;
        for (i=0; i<eCount; i++)
        {
            if (FD_ISSET(eArray[i],eSet))
            {
                eArray[j] = eArray[i];
                j++;
            }
        }
        eCount = j;
    }


    /*---------------------------------------------------------------
     * reset the stem variables
     *---------------------------------------------------------------*/
    if (rArray)
    {
        intarray2rxstem(context, array1,rCount,rArray);
    }
    if (wArray)
    {
        intarray2rxstem(context, array2,wCount,wArray);
    }
    if (eArray)
    {
        intarray2rxstem(eontex, array3,eCount,eArray);
    }

    /*---------------------------------------------------------------
     * free arrays
     *---------------------------------------------------------------*/
    if (rArray)
    {
        free(rArray);
    }
    if (wArray)
    {
        free(wArray);
    }
    if (eArray)
    {
        free(eArray);
    }

    /*---------------------------------------------------------------
     * set return code
     *---------------------------------------------------------------*/
    return rc;
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * send()
 *------------------------------------------------------------------*/
RexxFunction3(int, SockSend, int, sock, RexxStringObject, dataObj, OPTIONAL_CSTRING, flagArg)
{
    int      dataLen;
    char    *data;
    int      flags;
    long     rc;
    int      chk;


    /*---------------------------------------------------------------
     * get data length
     *---------------------------------------------------------------*/
    dataLen = context->StringLength(dataObj);
    data    = context->StringData(dataObj);

    /*---------------------------------------------------------------
     * get flags
     *---------------------------------------------------------------*/
    flags = 0;
    if (flagArg != NULL)
    {
        char *pszWord;

        pszWord = strtok(flagArg, " ");
        while (pszWord)
        {
            if (!stricmp(pszWord,"MSG_OOB"))
            {
                flags |= MSG_OOB;
            }
            else if (!stricmp(pszWord,"MSG_DONTROUTE"))
            {
                flags |= MSG_DONTROUTE;
            }

            pszWord = strtok(NULL," ");
        }
    }

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    rc = send(sock,data,dataLen,flags);

    /*---------------------------------------------------------------
     * set return code
     *---------------------------------------------------------------*/
    return rc;
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * sendto()
 *------------------------------------------------------------------*/
RexxFunction4(int, SockSendTo, int, sock, RexxStringObject, dataObj, RexxObjectPtr, flagsOrStem, OPTIONAL_RexxStemObject, stem)
{
    int      dataLen;
    char    *data;
    int      flags;
    long     rc;
    int      chk;
    sockaddr_in addr;

    /*---------------------------------------------------------------
     * get data length
     *---------------------------------------------------------------*/
    dataLen = context->StringLength(dataObj);
    data    = context->StringData(dataObj);

    /*---------------------------------------------------------------
     * get flags
     *---------------------------------------------------------------*/
    flags = 0;
    if (stem != NULLOBJECT)
    {
        char *pszWord = context->ObjectToStringValue(flagsOrStem);

        pszWord = strtok(pszWord, " ");
        while (pszWord)
        {
            if (!stricmp(pszWord,"MSG_DONTROUTE"))
            {
                flags |= MSG_DONTROUTE;
            }
            pszWord = strtok(NULL," ");
        }
    }
    else
    {
        stem = context->ResolveContextStemVariable(flagsOrStem);
    }

    stem2sockaddr(stem, &addr);

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    return = sendto(sock,data,dataLen,flags,(struct sockaddr *)&addr,sizeof(addr));
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/
/*------------------------------------------------------------------
 * setsockopt()
 *------------------------------------------------------------------*/
RexxFunction4(int, SockSetSockOpt, int, sock, CSTRING, target, CSTRING, option, CSTRING, arg)
{
    int            opt;
    struct linger  lingStruct;
    int            intVal;
    long           longVal;
    long           longVal1;
    long           longVal2;
    int            len;
    void          *ptr;


    if (stricmp("SOL_SOCKET", target))
    {
        context->InvalidRoutine();
        return 0;
    }

    /*---------------------------------------------------------------
     * get option name
     *---------------------------------------------------------------*/
    opt = rxs2SockOpt(option);

    /*---------------------------------------------------------------
     * get option value
     *---------------------------------------------------------------*/
    switch (opt)
    {
        default:
            ptr = &intVal;
            len = sizeof(int);
            sscanf(arg, "%d", &intVal);
            break;

        case SO_LINGER:
            ptr = &lingStruct;
            len = sizeof(lingStruct);

            sscanf(arg,"%ld %ld",&longVal1,&longVal2);
            lingStruct.l_onoff  = (u_short)longVal1;
            lingStruct.l_linger = (u_short)longVal2;

            break;

        case SO_RCVBUF:
        case SO_SNDBUF:
            ptr = &longVal;
            len = sizeof(long);

            sscanf(arg, "%ld", &longVal);
            break;

        case SO_ERROR:
        case SO_TYPE:
            return -1;
    }

    /*---------------------------------------------------------------
     * make call
     *---------------------------------------------------------------*/
    return setsockopt(sock,SOL_SOCKET,opt,ptr,len);
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * shutdown()
 *------------------------------------------------------------------*/
RexxFunction2(int, SockShutDown, int, sock, int, how)
{
    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    return shutdown(sock, how);
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 *  sock_init()
 *------------------------------------------------------------------*/
RexxFunction0(int, SockInit)
{
#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD( 1, 1 );
    return WSAStartup( wVersionRequested, &wsaData );
#else
    return 0;
#endif
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * socket()
 *------------------------------------------------------------------*/
RexxFunction3(int, SockSocket, CSTRING, domainArg, CSTRING, typeArg, CSTRING, protocolArg)
{
    int domain;
    int type;
    int protocol;
    char *pszDomain;
    char *pszType;
    char *pszProtocol;
    int sock;

    /*---------------------------------------------------------------
     * get parms
     *---------------------------------------------------------------*/
    pszDomain   = strdup(domainArg);
    pszType     = strdup(typeArg);
    pszProtocol = strdup(ProtcolArg);

    stripBlanks(pszDomain);
    stripBlanks(pszType);
    stripBlanks(pszProtocol);

    if (!stricmp(pszDomain,"AF_INET"))
    {
        domain = AF_INET;
    }
    else
    {
        context->InvalidRoutine();
        return 0;
    }

    if (!stricmp(pszType,"SOCK_STREAM")) type = SOCK_STREAM;
    else if (!stricmp(pszType,"SOCK_DGRAM" )) type = SOCK_DGRAM;
    else if (!stricmp(pszType,"SOCK_RAW"   )) type = SOCK_RAW;
    else
    {
        context->InvalidRoutine();
        return 0;
    }

    if (!stricmp(pszProtocol,"IPPROTO_UDP"))
        protocol = IPPROTO_UDP;
    else if (!stricmp(pszProtocol,"IPPROTO_TCP"))
        protocol = IPPROTO_TCP;
    else if (!stricmp(pszProtocol,"0"          ))
        protocol = 0;
    else
    {
        context->InvalidRoutine();
        return 0;
    }

    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
    return socket(domain,type,protocol);
}

/*-/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\-*/
/*-\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/-*/

/*------------------------------------------------------------------
 * soclose()
 *------------------------------------------------------------------*/
RexxFunction1(int, SockSoClose, int, sock)
{
    /*---------------------------------------------------------------
     * call function
     *---------------------------------------------------------------*/
#if defined(WIN32)
    return closesocket(sock);
#else
    return close(sock);
#endif
}

