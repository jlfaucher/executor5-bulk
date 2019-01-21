/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2019 Rexx Language Association. All rights reserved.    */
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


#ifndef FileNameBuffer_Included
#define FileNameBuffer_Included

#include "SysFileSystem.hpp"

/**
 * A class for performing safer file name resolution that is expandable if needed. Replaces the
 * use of fixed-sized buffers for file operations.
 *
 * NOTE: This can only be used within code that runs
 * while the interpreter has control.
 */
class FileNameBuffer
{
 public:
     FileNameBuffer(size_t initial = 0);
     inline ~FileNameBuffer()
     {
         if (buffer != NULL)
         {
             delete buffer;
             buffer = NULL;
         }
     }

     virtual void handleMemoryError();
     virtual FileNameBuffer* allocateNewBuffer();

     size_t capacity() { return bufferSize; }
     size_t length() { return strlen(buffer); }

     // cast conversion operators
     inline operator char *()
     {
         return buffer;
     }

     // cast conversion operators
     inline operator const char *()
     {
         return (const char *)buffer;
     }

     void ensureCapacity(size_t size);
     inline void ensureCapacity(const char *add) { ensureCapacity(strlen(buffer) + strlen(add) + 1); }
     inline void expandCapacity(size_t c)  { ensureCapacity(bufferSize + c); }
     inline void shiftLeft(size_t l)
     {
         size_t len = length();
         // longer than the length? This becomes a null string
         if (l > len)
         {
             buffer[0] = '\0';
         }
         else
         {
             // move the remainder plus the null.
             memmove(buffer, buffer + l, len - l + 1);
         }
     }

     inline FileNameBuffer &operator=(const char *s)
     {
         ensureCapacity(strlen(s));
         strncpy(buffer, s, bufferSize);
         return *this;
     }

     inline FileNameBuffer &operator=(char *s)
     {
         ensureCapacity(strlen(s));
         strncpy(buffer, s, bufferSize);
         return *this;
     }

     inline FileNameBuffer &operator+=(const char *s)
     {
         ensureCapacity(s);
         strncat(buffer, s, bufferSize);
         return *this;
     }

     inline FileNameBuffer &operator+=(char *s)
     {
         ensureCapacity(s);
         strncat(buffer, s, bufferSize);
         return *this;
     }

     inline FileNameBuffer& set(const char *s, size_t l)
     {
         ensureCapacity(l);
         memcmp(buffer, s, l);
         buffer[l] = '\0';
     }

     inline FileNameBuffer& append(const char *s, size_t l)
     {
         size_t currentLength = length();
         ensureCapacity(currentLength + l);
         memcmp(buffer + currentLength, s, l);
         buffer[currentLength + l] = '\0';
     }

     // this is a mutable request, so we need to ensure the position is within the
     // current buffer size
     inline char & operator [](size_t pos)
     {
         ensureCapacity(pos + 1);
         return *(buffer + pos);
     }

 protected:
     char *buffer;                 // the current buffer
     size_t bufferSize;            // the current buffer size

};


/**
 * Simple class for resolving qualified file names.
 *
 * NOTE: This can only be used within code that runs
 * while the interpreter has control.
 */
class QualifiedName
{
 public:
     QualifiedName(const char *name)
     {
         SysFileSystem::qualifyStreamName(name, qualifiedName);
     }

     // cast conversion operators for some very common uses of protected object.
     inline operator const char *()
     {
         return (const char *)qualifiedName;
     }


 protected:
     // buffer for holding the qualified name
     FileNameBuffer qualifiedName;

};


/**
 * A simple implemenation of a smart pointer to prevent memory
 * leaks with dynamically allocated FileNameBuffer objects
 */
class AutoFileNameBuffer
{
 public:
     AutoFileNameBuffer(FileNameBuffer &b)
     {
         // get a buffer of the same type as the input one
         buffer = b.allocateNewBuffer();
     };

     ~AutoFileNameBuffer()
     {
         delete buffer;
     }

     // . method access. Note that . cannot be overridden,
     // but we can override ->, so method access will need be
     // done using pointer notation.
     inline FileNameBuffer &operator->()
     {
         return *buffer;
     }

     // cast conversion operators
     inline operator FileNameBuffer &()
     {
         return *buffer;
     }


     // cast conversion operators
     inline operator char *()
     {
         return (char *)buffer;
     }

     // cast conversion operators
     inline operator const char *()
     {
         return (const char *)buffer;
     }

     inline FileNameBuffer &operator=(const char *s)
     {
         return *buffer = s;
     }

     inline FileNameBuffer &operator=(char *s)
     {
         return *buffer = s;
     }

     inline FileNameBuffer &operator+=(const char *s)
     {
         return *buffer += s;
     }

     inline FileNameBuffer &operator+=(char *s)
     {
         return *buffer += s;
     }

     // this is a mutable request, so we need to ensure the position is within the
     // current buffer size
     inline char & operator [](size_t pos)
     {
         return buffer->operator[](pos);
     }


 private:
     FileNameBuffer *buffer;
};
#endif
