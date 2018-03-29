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
/******************************************************************************/
/*                                                       OutputRedirector.hpp */
/*                                                                            */
/* Implementations of the different ADDRESS WITH Output redirectors           */
/*                                                                            */
/******************************************************************************/
#ifndef Included_OutputRedirector
#define Included_OutputRedirector


/**
 * Base class for I/O redirectors.
 */
class OutputRedirector : public RexxInternalObject
{
 public:

     // the options for how how output is handled
     enum
     {
         DEFAULT,
         APPEND,
         REPLACE
     } OutputOption;

    // Note, we never directly create instances of this
    // class so we don't need new, live, etc. These must
    // be implemented by the concrete classes
    inline OutputRedirector() { ; }
    inline OutputRedirector(RESTORETYPE restoreType) { ; };

    virtual void init (OutputOption option) { ; }
    virtual void write(const char *data)  { ; }
};


/**
 * Output handler for collecting lines in a stem object.
 */
class StemOutputTarget : public OutputRedirector
{
 public:
    void        *operator new(size_t, size_t);
    inline void  operator delete(void *) { }

    // Note, we never directly create instances of this
    // class so we don't need new, live, etc. These must
    // be implemented by the concrete classes
    inline OutputRedirector(StemClass *stem);
    inline OutputRedirector(RESTORETYPE restoreType) { ; };

    virtual void live(size_t);
    virtual void liveGeneral(MarkReason reason);

    virtual void init (OutputOption option);
    virtual void write(const char *data);

protected:
    StemClass *stem;      // the stem we're handling
    size_t     index;     // current index we're writing to
};


/**
 * Output handler for collecting lines in a stem object.
 */
class StemOutputTarget : public OutputRedirector
{
 public:
    void        *operator new(size_t, size_t);
    inline void  operator delete(void *) { }

    inline StemOutputTarget(StemClass *stem);
    inline StemOutputTarget(RESTORETYPE restoreType) { ; };

    virtual void live(size_t);
    virtual void liveGeneral(MarkReason reason);

    virtual void init (OutputOption option);
    virtual void write(const char *data);

protected:
    StemClass *stem;      // the stem we're handling
    size_t     index;     // current index we're writing to
};


/**
 * Output handler for collecting lines in an OutputStream object
 *
 */
class StreamObjectOutputTarget : public OutputRedirector
{
 public:
    void        *operator new(size_t, size_t);
    inline void  operator delete(void *) { }

    inline StreamObjectOutputTarget(RexxObject *s);
    inline StreamObjectOutputTarget(RESTORETYPE restoreType) { ; };

    virtual void live(size_t);
    virtual void liveGeneral(MarkReason reason);

    virtual void init (OutputOption option);
    virtual void write(const char *data);

protected:
    RexxObject *stream;   // the stream object
};


/**
 * Output handler for collecting lines in a named stream
 *
 */
class StreamOutputTarget : public StreamObjectOutputTarget
{
 public:
    void        *operator new(size_t, size_t);
    inline void  operator delete(void *) { }

    inline StreamOutputTarget(RexxString *n);
    inline StreamOutputTarget(RESTORETYPE restoreType) { ; };

    virtual void live(size_t);
    virtual void liveGeneral(MarkReason reason);

    virtual void init (OutputOption option);
    virtual void write(const char *data);

protected:
    RexxString *name;     // the stream name
};


/**
 * Output handler for collecting lines in an ordered collection
 *
 */
class CollectionOutputTarget : public OutputRedirector
{
 public:
    void        *operator new(size_t, size_t);
    inline void  operator delete(void *) { }

    inline CollectionOutputTarget(RexxObject *c);
    inline CollectionOutputTarget(RESTORETYPE restoreType) { ; };

    virtual void live(size_t);
    virtual void liveGeneral(MarkReason reason);

    virtual void init (OutputOption option);
    virtual void write(const char *data);

protected:
    RexxObject *collection;  // the target collection object
};
#endif

