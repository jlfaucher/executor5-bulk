/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
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
/******************************************************************************/
/*                                                                            */
/* Non-platform-specific rexxutil functions                                   */
/*                                                                            */
/******************************************************************************/

#include "oorexxapi.h"
#include "PackageManager.hpp"
#include "RexxUtilCommon.hpp"
#include "RexxInternalApis.h"

/**
 * A simple class for reading lines from a file.
 */
class LineReader
{
 public:
     LineReader() buffer(null), bufferSize(0), fileSize(0), dataLength(0),
     fileResidual(0), scan(null), file()
     { }
     ~LineReader()
     {
         close();
     }

     /**
      * Open the file and set up for reading
      *
      * @param fileName The name of the file
      *
      * @return true if we we able to open the file and prepare for reading. false for any failures.
      */
     bool open(const char *fileName)
     {
         // if we can't open the file, return
         if (!file.open(fileName, RX_O_RDONLY, RX_SH_DENY_WR, RX_S_IREAD))
         {
             return false;
         }
         // also need to determine a size
         if (~file.getSize(fileSize))
         {
             return false;
         }

         // we read every thing initially
         fileResidual = fileSize;
         bufferSize = min(fileSize, InitialBufferSize);
         // allocate a buffer to hold the entire file.
         buffer = (char *)malloc(bufferSize);
         if (buffer == NULL)
         {
             return false;
         }

         // go read the next (first) buffer of data
         return readNextBuffer();
     }


     /**
      * Read the next buffer of data. This might be a partial read because
      * there is still residual data in the buffer. In that case, the
      * data will be shifted to the front and more data added to the end.
      *
      * @return true if this worked, false otherwise
      */
     bool readNextBuffer()
     {
         // iif nothing left, return a failure
         if (fileResidual == 0)
         {
             return false;
         }

         char *readLocation = buffer;
         size_t readLength = bufferSize;

         // do we have data still in the buffer? We need to shift this to the front.
         if (dataLength > 0)
         {
             // shift the data in the buffer
             memmove(buffer, scan, dataLength);
             readLocation = buffer + dataLength;
             readLength = bufferSize - dataLength;
         }

         readLength = min(fileResidual, readLength)
         // if the read fails or we don't get anything, return a failure
         if (!file.read(buffer, readLength, dataLength) || dataLength == 0)
         {
             return false;
         }

         // if we got less than expected, we're done reading
         if (dataLength < readLength)
         {
             fileResidual = 0;
         }
         else
         {
             // adjust the residual size for the amount read
             fileResidual -= dataLength;
         }

         // scan for an eof character, if there is one
         char *endptr = (char *)memchr(buffer, CH_EOF, dataLength);
         // if we have a hit, then we need to adjust the length downward
         if (endptr != NULL)
         {
             // if we hit the EOF, then we ignore the rest of the file
             dataLength = endptr - buffer;
             fileResidual = 0;
         }
         // set scan position to the beginning
         scan = buffer;
         return true;
     }


     /**
      * Increase the size of the buffer because we have a line longer than the
      * size of the buffer.
      *
      * @return true if this worked, false otherwise
      */
     bool expandBuffer()
     {
         // iif nothing left, return a failure
         if (fileResidual == 0)
         {
             return false;
         }

         size_t readOffset = bufferSize;
         size_t newBufferSize = bufferSize + BufferExpansionSize;

         // try to expand
         char *newBuffer = realloc(buffer, newBufferSize);
         // can't expand, then just return the end
         if (newBuffer == NULL)
         {
             return false;
         }

         // we're reallocated, adjust for the new reality
         buffer = newBuffer;
         bufferSize = newBufferSize;
         scan = buffer + readOffset:

         // read some more data
         return readNextBuffer();
     }


     /**
      * Get the next line of the file, returning a pointer to the data
      * and also the line length
      *
      * @param line   The returned line pointer
      * @param size   The return line size
      *
      * @return true if a line is read, false otherwise
      */
     bool getLine(const char *&line, size_t &size)
     {
         // if we're out of data, ten read the next buffer, unless there's
         // nothing left of the file
         if (dataLength == 0)
         {
             if (!readNextBuffer())
             {
                 return false;
             }
         }
         // look for a line
         if (findLine(line, size))
         {
             return true;
         }
         // ok, we have some data in the buffer, but no linend. First, adjust the buffer by
         // shifting the remainder of the buffer to the front and reading more data from the file.
         readNextBuffer();
         // look for a line in the adjusted buffer
         if (findLine(line, size))
         {
             return true;
         }
         // even with more data, we didn't fine a line end. Try expanding the buffer and
         // try again (and again, and again, as long as we can)
         while (expandBuffer())
         {
             // look for a line in the adjusted buffer
             if (findLine(line, size))
             {
                 return true;
             }
         }

         // no longer possible to expand the buffer... return what we have
         line = scan;
         size = dataLength;

         return true;
     }


     /**
      * Find a line in the current buffer and return the location.
      *
      * @param line   The returned line pointer
      * @param size   The returne line size
      *
      * @return true if the line was found, false otherwise.
      */
     bool findLine(const char *&line, size_t &size)
     {
         // look for a line feed character for the end of the last line
         const char *linend = (const char *)memchr(scan, CH_NL, dataLength);
         // did we get a hit?
         if (linend != NULL)
         {
             // calculate the return length
             size = linend - scan;
             // this is the start of the line
             line = scan;

             // reduce the length and bump the scan position
             dataLength -= size + 1;
             scan = linend + 1''

             // we don't want the CR character in the result string
             if (*(linend - 1) == CH_CR)
             {
                 size--;
             }
             // we got something
             return true;
         }
     }

     /**
      * Close the read operation.
      */
     void close()
     {
         if (buffer != NULL)
         {
             free(buffer);
         }
         file.close();
     }


 protected:
     // the initial size of our buffer
     const size_t InitialBufferSize = 0x0001000;
     // amount we extend the buffer by if a line is longer
     const size_t BufferExpansionSize = 0x0000800;

     char *buffer;                 // the current buffer
     size_t bufferSize;            // current size of the buffer
     size_t fileSize;              // full size of the file
     size_t dataLength;            // the data left in the buffer
     size_t fileResidual;          // the amount of unread data in the file
     const char  *scan;            // current scan postion in the buffer
     SysFile file;                 // the file information we are reading
};


/**
 * Determines if the string 'needle' is in the
 * string 'haystack' by returning it's position or
 * a NULL if not found.  The length of haystack and
 * needle are given by 'hlen' and 'nlen' respectively.
 *
 * If 'sensitive' is true, then the search is case
 * sensitive, else it is case insensitive.
 *
 * @param haystack  The haystack to search in.
 * @param needle    The needle to search for
 * @param hlen      the length of the haystack string
 * @param nlen      The lenght of the needle string
 * @param sensitive if true, a case sensitive search is performed. if false, the search is case insensitive.
 *
 * @return A pointer to the first match position or NULL if the string is not found.
 */
const char* mystrstr(const char *haystack, const char *needle, size_t hlen, size_t nlen, bool sensitive)
{
    // we never return a hit for a null string search or if the needle is longer than the haystack
    if (nlen == 0 || nlen > hlen)
    {
        return NULL;
    }

    // different loops for the sensitive and insensitive searches
    if ( sensitive )
    {
        // we scan, looking for a hit on the first character
        char firstChar = needle[0];

        size_t current = 0;
        // this is maximum number of places we can get a hit
        size_t limit = hlen - nlen + 1;
        for ( current = 0; current < limit; current++ )
        {
            // if we have a hit on the first character, check the entire string
            if ( firstChar == haystack[current] )
            {
                // if everything compares, return the hit
                if ( memcmp(haystack + current, needle, nlen) == 0 )
                {
                    return haystack + current;
                }
            }
        }
    }
    else
    {
        // we scan, looking for a hit on the first character
        char firstChar = toupper(needle[0]);

        size_t current = 0;
        // this is maximum number of places we can get a hit
        size_t limit = hlen - nlen + 1;
        for (current = 0; current < limit; current++)
        {
            // if we have a hit on the first character, check the entire string
            if (firstChar == toupper(haystack[current]))
            {
                // if everything compares, return the hit
                if (memicmp(haystack + current, needle, nlen) == 0)
                {
                    return haystack + current;
                }
            }
        }
    }
    // if the loops terminate, there are no hits
    return NULL;
}



/*************************************************************************
* Function:  SysDropFuncs                                                *
*                                                                        *
* Syntax:    call SysDropFuncs                                           *
*                                                                        *
* Return:    NO_UTIL_ERROR - Successful.                                 *
*************************************************************************/
RexxRoutine0(int, SysDropFuncs)
{
    // this is a nop
    return 0;
}


/*************************************************************************
* Function:  SysLoadFuncs                                                *
*                                                                        *
* Syntax:    call SysLoadFuncs [option]                                  *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    null string                                                 *
*************************************************************************/
RexxRoutine0(int, SysLoadFuncs)
{
    // this is a nop
    return 0;
}


/*************************************************************************
* Function:  SysAddRexxMacro                                             *
*                                                                        *
* Syntax:    result = SysAddRexxMacro(name, file, <order>)               *
*                                                                        *
* Params:    name   - loaded name of the macro file                      *
*            file   - file containing the loaded macro                   *
*            order  - Either 'B'efore or 'A'fter.  The default is 'B'    *
*                                                                        *
* Return:    return code from RexxAddMacro                               *
*************************************************************************/

RexxRoutine3(int, SysAddRexxMacro, CSTRING, name, CSTRING, file, OPTIONAL_CSTRING, option)
{
    size_t position;         /* added position             */

    position = RXMACRO_SEARCH_BEFORE;    /* set default search position*/
    if (option != NULL)                  /* have an option?            */
    {
        switch (*option)
        {
            case 'B':     // 'B'efore
            case 'b':
                position = RXMACRO_SEARCH_BEFORE;
                break;

            case 'A':     // 'A'fter
            case 'a':
                position = RXMACRO_SEARCH_AFTER;
                break;

            default:
                context->InvalidRoutine();
                return 0;
        }
    }
    // try to add the macro
    return (int)RexxAddMacro(name, file, position);
}


/*************************************************************************
* Function:  SysReorderRexxMacro                                         *
*                                                                        *
* Syntax:    result = SysReorderRexxMacro(name, order)                   *
*                                                                        *
* Params:    name   - loaded name of the macro file                      *
*            order  - Either 'B'efore or 'A'fter.                        *
*                                                                        *
* Return:    return code from RexxReorderMacro                           *
*************************************************************************/
RexxRoutine2(int, SysReorderRexxMacro, CSTRING, name, CSTRING, option)
{
    size_t position;        /* added position             */

    switch (*option)
    {
        case 'B':     // 'B'efore
        case 'b':
            position = RXMACRO_SEARCH_BEFORE;
            break;

        case 'A':     // 'A'fter
        case 'a':
            position = RXMACRO_SEARCH_AFTER;
            break;

        default:
            context->InvalidRoutine();
            return 0;
    }
    return (int)RexxReorderMacro(name, position);
}


/*************************************************************************
* Function:  SysDropRexxMacro                                            *
*                                                                        *
* Syntax:    result = SysDropRexxMacro(name)                             *
*                                                                        *
* Params:    name   - name of the macro space function                   *
*                                                                        *
* Return:    return code from RexxDropMacro                              *
*************************************************************************/
RexxRoutine1(int, SysDropRexxMacro, CSTRING, name)
{
   return (int)RexxDropMacro(name);
}


/*************************************************************************
* Function:  SysQueryRexxMacro                                           *
*                                                                        *
* Syntax:    result = SysQueryRexxMacro(name)                            *
*                                                                        *
* Params:    name   - name of the macro space function                   *
*                                                                        *
* Return:    position of the macro ('B' or 'A'), returns null for errors.*
*************************************************************************/
RexxRoutine1(CSTRING, SysQueryRexxMacro, CSTRING, name)
{
    unsigned short position;         /* returned position          */

    if (RexxQueryMacro(name, &position) != 0)
    {
        return "";
    }
    // before?
    if (position == RXMACRO_SEARCH_BEFORE)
    {
        return "B";
    }
    else
    {
        return "A";                    /* must be 'A'fter            */
    }
}


/*************************************************************************
* Function:  SysClearRexxMacroSpace                                      *
*                                                                        *
* Syntax:    result = SysClearRexxMacroSpace()                           *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    return code from RexxClearMacroSpace()                      *
*************************************************************************/
RexxRoutine0(int, SysClearRexxMacroSpace)
{
    return (int)RexxClearMacroSpace();          /* clear the macro space      */
}

/*************************************************************************
* Function:  SysSaveRexxMacroSpace                                       *
*                                                                        *
* Syntax:    result = SysSaveRexxMacroSpace(file)                        *
*                                                                        *
* Params:    file   - name of the saved macro space file                 *
*                                                                        *
* Return:    return code from RexxSaveMacroSpace()                       *
*************************************************************************/

RexxRoutine1(int, SysSaveRexxMacroSpace, CSTRING, file)
{
    return (int)RexxSaveMacroSpace(0, NULL, file);
}

/*************************************************************************
* Function:  SysLoadRexxMacroSpace                                       *
*                                                                        *
* Syntax:    result = SysLoadRexxMacroSpace(file)                        *
*                                                                        *
* Params:    file   - name of the saved macro space file                 *
*                                                                        *
* Return:    return code from RexxLoadMacroSpace()                       *
*************************************************************************/

RexxRoutine1(int, SysLoadRexxMacroSpace, CSTRING, file)
{
    return (int)RexxLoadMacroSpace(0, NULL, file);
}


/*************************************************************************
* Function:  SysStemSort                                                 *
*                                                                        *
* Syntax:    result = SysStemSort(stem, order, type, start, end,         *
*                                 firstcol, lastcol)                     *
*                                                                        *
* Params:    stem - name of stem to sort                                 *
*            order - 'A' or 'D' for sort order                           *
*            type - 'C', 'I', 'N' for comparision type                   *
*            start - first index to sort                                 *
*            end - last index to sort                                    *
*            firstcol - first column to use as sort key                  *
*            lastcol - last column to use as sort key                    *
*                                                                        *
* Return:    0 - sort was successful                                     *
*            -1 - sort failed                                            *
*************************************************************************/

RexxRoutine7(int, SysStemSort, RexxStringObject, stem, OPTIONAL_CSTRING, order, OPTIONAL_CSTRING, type, OPTIONAL_size_t, first, OPTIONAL_size_t, last,
    OPTIONAL_size_t, firstCol, OPTIONAL_size_t, lastCol)
{
    int           sortType = SORT_CASESENSITIVE;
    int           sortOrder = SORT_ASCENDING;

    char          stemName[255];


    // This is a bit of a pain. It would be nice to create this argument as a stem
    // object, but people have been coding stem sorts using a compound variable rather than
    // a stem name because the old code was lax and it just happened to work. We need to process this
    // as a name and make sure it has a period at the end.
    memset(stemName, 0, sizeof(stemName));
    strncpy(stemName, stem);
    if (stemName[strlen(stem) - 1] != '.')
    {
        stemName[strlen(stem)] = '.';
    }

    // check other parameters.  sort order
    if ( order != NULL)
    {
        switch ( order[0] )
        {
            case 'A':
            case 'a':
                sortOrder = SORT_ASCENDING;
                break;
            case 'D':
            case 'd':
                sortOrder = SORT_DECENDING;
                break;
            default:
                context->InvalidRoutine();
                return 0;
        }
    }

    // sort type
    if (type != NULL)
    {
        switch (type[0])
        {
            case 'C':
            case 'c':
                sortType = SORT_CASESENSITIVE;
                break;
            case 'I':
            case 'i':
                sortType = SORT_CASEIGNORE;
                break;
            default:
                context->InvalidRoutine();
                return 0;
        }
    }

    // first element to sort
    if (argumentExists(4))
    {
        // must be positive
        if ( first == 0 )
        {
            context->InvalidRoutine();
            return 0;
        }
    }
    // start with the first element if not specified
    else
    {
        first = 1;
    }

    // last element to sort
    if (argumentExists(5))
    {
        // can't go backwards
        if (last < first)
        {
            context->InvalidRoutine();
            return 0;
        }
    }
    // do everything
    else
    {
        last = SIZE_MAX;
    }

    // first column to sort
    if (argumentExists(6))
    {
        // must be positive
        if ( firstCol == 0 )
        {
            context->InvalidRoutine();
            return 0;
        }
        firstCol;
    }
    else
    {
        firstCol = 1;
    }


    // last column to sort
    if (argumentExists(7))
    {
        if (lastCol == 0 || lastCol < firstCol)
        {
            context->InvalidRoutine();
            return 0;
        }
    }
    else
    {
        lastCol = SIZE_MAX
    }

    // the sorting is done in the interpreter
    if ( !RexxStemSort(stemName, sortOrder, sortType, first, last, firstCol, lastCol) )
    {
        context->InvalidRoutine();
    }

    return 0;
}


/*************************************************************************
* Function:  SysStemDelete                                               *
*                                                                        *
* Syntax:    result = SysStemDelete(stem, startitem [,itemcount])        *
*                                                                        *
* Params:    stem - name of stem where item will be deleted              *
*            startitem - index of item to delete                         *
*            itemcount - number of items to delete if more than 1        *
*                                                                        *
* Return:    0 - delete was successful                                   *
*            -1 - delete failed                                          *
*************************************************************************/
RexxRoutine3(int, SysStemDelete, RexxStemObject, toStem, stringsize_t, start, OPTIONAL_stringsize_t, count)
{
    if (argumentOmitted(3))
    {
        count = 1;
    }

    stringsize_t items;

    RexxObjectPtr temp = context->GetStemArrayElement(toStem, 0);
    if (temp == NULLOBJECT || !context->StringSize(temp, &items))
    {
        context->InvalidRoutine();
        return 0;
    }

    // make sure the deletion site is within the bounds
    if (start + count - 1 > items)
    {
        context->InvalidRoutine();
        return 0;
    }

    stringsize_t index;
    /* now copy the remaining indices up front */
    for ( index = start;  index + count <= items; index++)
    {
        // copy from the old index to the new index
        RexxObjectPtr value = context->GetStemArrayElement(toStem, index + count);
        // is this a sparse array?
        if (value == NULLOBJECT)
        {
            // return this as a failure
            return -1;
        }
        context->SetStemArrayElement(toStem, index, value);
    }

    /* now delete the items at the end */
    for (index = items - count + 1; index <= items; index++)
    {
        context->DropStemArrayElement(toStem, index);
    }

    context->SetStemArrayElement(toStem, 0, context->StringSize(items - count));
    return 0;
}


/*************************************************************************
* Function:  SysStemInsert                                               *
*                                                                        *
* Syntax:    result = SysStemInsert(stem, position, value)               *
*                                                                        *
* Params:    stem - name of stem where item will be inserted             *
*            position - index where new item will be inserted            *
*            value - new item value                                      *
*                                                                        *
* Return:    0 - insert was successful                                   *
*            -1 - insert failed                                          *
*************************************************************************/
RexxRoutine3(int, SysStemInsert, RexxStemObject, toStem, stringsize_t, position, RexxObjectPtr, newValue)
{
    stringsize_t count;

    RexxObjectPtr temp = context->GetStemArrayElement(toStem, 0);
    if (temp == NULLOBJECT || !context->StringSize(temp, &count))
    {
        context->InvalidRoutine();
        return 0;
    }

    /* check wether new position is within limits */
    if (position == 0 || (position > count + 1))
    {
        context->InvalidRoutine();
        return 0;
    }

    for (size_t index = count; index >= position; index--)
    {
        // copy from the old index to the new index
        RexxObjectPtr value = context->GetStemArrayElement(toStem, index);
        // is this a sparse array?
        if (value == NULLOBJECT)
        {
            // return this as a failure
            return -1;
        }
        context->SetStemArrayElement(toStem, index + 1, value);
    }

    // now set the new value and increase the count at stem.0
    context->SetStemArrayElement(toStem, position, newValue);
    context->SetStemArrayElement(toStem, 0, context->WholeNumber(count + 1));
    return 0;
}


/*************************************************************************
* Function:  SysStemCopy                                                 *
*                                                                        *
* Syntax:    result = SysStemCopy(fromstem, tostem, from, to, count      *
*                                 [,insert])                             *
*                                                                        *
* Params:    fromstem - name of source stem                              *
*            tostem - - name of target stem                              *
*            from  - first index in source stem to copy                  *
*            to - position where items are copied/inserted in target stem*
*            count - number of items to copy/insert                      *
*            insert - 'I' to indicate insert instead of 'O' overwrite    *
*                                                                        *
* Return:    0 - stem copy was successful                                *
*            -1 - stem copy failed                                       *
*************************************************************************/
RexxRoutine6(int, SysStemCopy, RexxStemObject, fromStem, RexxStemObject, toStem,
    OPTIONAL_stringsize_t, from, OPTIONAL_stringsize_t, to, OPTIONAL_stringsize_t, count,
    OPTIONAL_CSTRING, option)
{
    bool inserting = false;

    /* get copy type */
    if (option != NULL)
    {
        switch (*option)
        {
            case 'I':
            case 'i':
                inserting = true;
                break;
            case 'O':
            case 'o':
                inserting = false;
                break;
            default:
            {
                context->InvalidRoutine();
                return 0;
            }
        }
    }

    stringsize_t fromCount;

    RexxObjectPtr temp = context->GetStemArrayElement(fromStem, 0);
    if (temp == NULLOBJECT || !context->StringSize(temp, &fromCount))
    {
        context->InvalidRoutine();
        return 0;
    }

    // default from location is the first element
    if (argumentOmitted(3))
    {
        from = 1;
    }

    if (argumentOmitted(4))
    {
        to = 1;
    }

    // was a count explicitly specified?
    if (argumentExists(5))
    {
        // this must be in range
        if ((count > (fromCount - from + 1)) || (fromCount == 0))
        {
            context->InvalidRoutine();
            return 0;
        }
    }
    else
    {
        // default is to copy everything from the starting position.
        count = fromCount - from + 1;
    }

    stringsize_t toCount = 0;
    // but if it is set, then use that value
    temp = context->GetStemArrayElement(toStem, 0);
    if (temp != NULLOBJECT && !context->StringSize(temp, &toCount))
    {
        context->InvalidRoutine();
        return 0;
    }

    // copying out of range?  Error
    if (to > toCount + 1)
    {
        context->InvalidRoutine();
        return 0;
    }

    if (inserting)
    {
        /* if we are about to insert the items we have to make room */
        for (size_t index = toCount; index >= to; index--)
        {
            // copy from the old index to the new index
            RexxObjectPtr value = context->GetStemArrayElement(toStem, index);
            // is this a sparse array?
            if (value == NULLOBJECT)
            {
                // return this as a failure
                return -1;
            }
            context->SetStemArrayElement(toStem, index + count, value);
        }


        // set the new count value in the target
        toCount += count;
        context->SetStemArrayElement(toStem, 0, context->StringSize(toCount));
    }
    /* now do the actual copying from the source to target */
    for (size_t index = 0; index < count; index++)
    {
        // just retrieve and copy
        RexxObjectPtr value = context->GetStemArrayElement(fromStem, from + index);
        // is this a sparse array?
        if (value == NULLOBJECT)
        {
            // return this as a failure
            return -1;
        }
        context->SetStemArrayElement(toStem, to + index, value);
    }

    // do we need to update the size?
    if (to + count - 1 > toCount)
    {
        context->SetStemArrayElement(toStem, 0, context->StringSize(to + count - 1));
    }
    return 0;
}


/*************************************************************************
* Function:  SysUtilVersion                                              *
*                                                                        *
* Syntax:    Say SysUtilVersion                                          *
*                                                                        *
* Return:    REXXUTIL.DLL Version                                        *
*************************************************************************/
RexxRoutine0(RexxStringObject, SysUtilVersion)
{
    char buffer[256];
                                       /* format into the buffer     */
    snprintf(buffer, sizeof(buffer), "%d.%d.%d", ORX_VER, ORX_REL, ORX_MOD);
    return context->String(buffer);
}


/**
 * Write out the name/value pair for an individual variable to the output stream.
 *
 * @param file    The output target
 * @param context The call context
 * @param name    The name of the variable
 * @param value   The object value
 *
 * @return an error code if there is a failure.
 */
int writeVariable(SysFile &file, RexxCallContext *context, RexxStringObject name, RexxObjectPtr value)
{
    size_t nameLength = context->StringLength(name);
    const char *nameData = context->StringData(name);

    RexxStringObject stringValue = context->ObjectToString(value);

    size_t valueLength = context->StringLength(value);
    const char *valueData = context->StringData(value);

    size_t bytesWritten;

    file.write("Name=", sizeof("Name="), bytesWritten, NULL);
    file.write(nameData, nameLength, bytesWritten, NULL);
    file.write(", Value='", sizeof(", Value='"), bytesWritten, NULL);
    file.write(valueData, valueLength, bytesWritten, NULL);
    file.write("'\r\n", sizeof("'\r\n"), bytesWritten, NULL);

    // now release the local references

    context->ReleaseLocalReference(name);
    context->ReleaseLocalReference(stringValue);
    context->ReleaseLocalReference(value);
}


/**
 * Write out the compound name/value pair for an individual
 * compound variable to the output stream.
 *
 * @param file   The output target
 * @param name   The name of the variable
 * @param value  The object value
 *
 * @return an error code if there is a failure.
 */
int writeVariable(SysFile &file, RexxCallContext *context, const char *stem, RexxStringObject tail, RexxObjectPtr value)
{
    size_t nameLength = context->StringLength(tail);
    const char *nameData = context->StringData(tail);

    RexxStringObject stringValue = context->ObjectToString(value);

    size_t valueLength = context->StringLength(value);
    const char *valueData = context->StringData(value);

    size_t bytesWritten;

    file.write("Name=", sizeof("Name="), bytesWritten, NULL);
    file.write(stem, strlen(stem), bytesWritten, NULL);
    file.write(nameData, nameLength, bytesWritten, NULL);
    file.write(", Value='", sizeof(", Value='"), bytesWritten, NULL);
    file.write(valueData, valueLength, bytesWritten, NULL);
    file.write("'\r\n", sizeof("'\r\n"), bytesWritten, NULL);

    // now release the local references

    context->ReleaseLocalReference(tail);
    context->ReleaseLocalReference(stringValue);
    context->ReleaseLocalReference(value);
}


/*************************************************************************
* Function:  SysDumpVariables                                            *
*                                                                        *
* Syntax:    result = SysDumpVariables([filename])                       *
*                                                                        *
* Params:    filename - name of the file where variables are appended to *
*                       (dump is written to stdout if omitted)           *
*                                                                        *
* Return:    0 - dump completed OK                                       *
*            -1 - failure during dump                                    *
*************************************************************************/
RexxRoutine1(int, SysDumpVariables, OPTIONAL_CSTRING, fileName)
{
    SysFile   outFile;;
    bool      closeFile = false;

    if (fileName != NULL)
    {
        QualifiedName qualifiedName(fileName);

        if (!outfile.open(qualifiedName, RX_O_WRONLY | RX_O_APPEND | RX_O_CREAT, RX_S_IWUSR | RX_S_IRUSR | RX_S_IWGRP | RX_S_IRGRP | RX_S_IWOTH | RX_S_IROTH))
        {
            context->InvalidRoutine();
            return 0;
        }
        closeFile = true;
    }
    else
    {
        outFile.setStdOut();
    }

    // get a snapshot of the variables
    RexxDirectoryObject variables = context->getAllContextVariables();

    // get a supplier for the variables
    RexxSupplierObject variableSupplier = (RexxSupplierObject)context->SendMessage0(variables, "SUPPLIER");

    while (context->SupplierAvailable(variableSupplier))
    {
        RexxStringObject variableName = context->SupplierIndex(variableSupplier);
        CSTRING name = context->ObjectToStringValue(variableName);

        // if the name ends in a period, this is a stem. We need to get the stem object
        // and iterate over the elements.
        if (name[strlen(name) - 1] == '.')
        {
            // get the stem value
            RexxStemObject stem = (RexxStemObject)context->SupplierValue(variableSupplier);

            RexxObjectPtr stemValue = context->GetStemValue(stem);

            // write out the stem name first
            writeVariable(outFile, context, variableName, stemValue);

            // now iterate on the compound variables
            RexxDirectoryObject compoundVariables = context->GetAllStemElements(stem);

            // get a supplier for the variables
            RexxSupplierObject compoundSupplier = (RexxSupplierObject)context->SendMessage0(compoundVariables, "SUPPLIER");

            // now iterate over the compound elements
            while (context->SupplierAvailable(compoundSupplier))
            {
                RexxStringObject tailName = (RexxStringObject)context->SupplierIndex(compoundSupplier);
                RexxObjectPtr compoundValue = context->SupplierValue(compoundSupplier);

                writeVariable(outFile, context, name, tailName, compoundValue);

                context->SupplierNext(compoundSupplier);
            }

            // release the stem object reference. Other references have already been released by
            // writeVariable
            context->ReleaseLocalReference(stem);
        }
        // a simple variable
        else
        {
            // get the stem value
            RexxObjectPtr value = context->SupplierValue(variableSupplier);

            // and write it out
            writeVariable(outFile, context, variableName, value);
        }

        // step to the next variable
        context->SupplierNext(variableSupplier);
    }

    if (closeFile)
    {
        close(outFile);
    }

    return 0
}


/**
 * Test if the file exists
 */
RexxRoutine1(logical_t, SysFileExists, CSTRING, name)
{
    QualifiedName qualifiedName(name);

    return SysFileSystem::exists(qualifiedName);
}

/*************************************************************************
* Function:  SysIsFileLink                                               *
*                                                                        *
* Syntax:    call SysIsFileLink file                                     *
*                                                                        *
* Params:    file - file to check if it is a Link (Alias).               *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxRoutine1(logical_t, SysIsFileLink, CSTRING, file)
{
    QualifiedName qualifiedName(file);

    return SysFileSystem::isLink(qualifiedName);
}


/*************************************************************************
* Function:  SysIsFile                                                   *
*                                                                        *
* Syntax:    call SysFileExist file                                      *
*                                                                        *
* Params:    file - file to check existance of.                          *
*                                                                        *
* Return:    true if this is a file object                               *
*************************************************************************/
RexxRoutine1(logical_t, SysIsFile, CSTRING, file)
{
    QualifiedName qualifiedName(file);

    return SysFileSystem::isFile(QualifiedName);
}

/*************************************************************************
* Function:  SysDirExist                                                 *
*                                                                        *
* Syntax:    call SysDirExist dir                                        *
*                                                                        *
* Params:    dir - dir to check existance of.                            *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxRoutine1(logical_t, SysIsFileDirectory, CSTRING, file)
{
    QualifiedName qualifiedName(file);

    return SysFileSystem::isDirectory(file);
}


/*************************************************************************
* Function:  SysRmDir                                                    *
*                                                                        *
* Syntax:    call SysRmDir dir                                           *
*                                                                        *
* Params:    dir - Directory to be removed.                              *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*                                                                        *
*************************************************************************/
RexxRoutine1(int, SysRmDir, CSTRING, path)
{
    QualifiedName qualifiedName(path);

    return SysFileSystem::deleteDirectory(qualifiedName);
}

/*************************************************************************
* Function:  SysFileDelete                                               *
*                                                                        *
* Syntax:    call SysFileDelete file                                     *
*                                                                        *
* Params:    file - file to be deleted.                                  *
*                                                                        *
* Return:    Return code from DosDelete() function.                      *
*************************************************************************/

RexxRoutine1(int, SysFileDelete, CSTRING, path)
{
    QualifiedName qualifiedName(path);

    return SysFileSystem::deleteFile(qualifiedName);
}


/*************************************************************************
* Function:  SysFileSearch                                               *
*                                                                        *
* Syntax:    call SysFileSearch target, file, stem [, options]           *
*                                                                        *
* Params:    target  - String to search for.                             *
*            file    - Filespec to search.                               *
*            stem    - Stem variable name to place results in.           *
*            options - Any combo of the following:                       *
*                       'C' - Case sensitive search (non-default).       *
*                       'N' - Preceed each found string in result stem   *
*                              with it line number in file (non-default).*
*                                                                        *
* Return:    NO_UTIL_ERROR   - Successful.                               *
*            ERROR_NOMEM     - Out of memory.                            *
*************************************************************************/
RexxRoutine4(CSTRING, SysFileSearch, CSTRING, needle, CSTRING, file, RexxStemObject, stem, OPTIONAL_CSTRING, opts)
{
    bool        linenums = false;        // should line numbers be inclued in the output
    bool        sensitive = false;       // how should searchs be performed

    // was the option specified?
    if (opts != NULL)
    {
        if (strstr(opts, "N") || strstr(opts, "n")) linenums = true;

        if (strstr(opts, "C") || strstr(opts, "c")) sensitive = true;
    }

    LineReader fileSource;
    QualifiedName qualifiedName(file);

    // if we can't open, return the error indicator
    if (!fileSource.open(qualifiedName))
    {
        return ERROR_FILEOPEN;
    }

    const char *line;
    size_t lineLength;

    size_t needleLength = strlen(needle);

    size_t currentLine = 0;

    size_t currentStemIndex = 0;

    // keep reading while we find lines
    while (fileSource.getLine(line, lineLen))
    {
        currentLine++;
        const char *ptr = mystrstr(line, needle, lineLength, needleLength, sensitive);

        if (ptr != NULL)
        {
            if (linenums)
            {
                char lineNumber[16];
                snprintf(lineNumber, sizeof(lineNumber), "%d ", currentLine);

                size_t totalLineSize = strlen(lineNumber) + size;

                char *lineBuffer = (char *)malloc();
                if (lineBuffer == NULL)
                {
                    // make sure we update the count with the number of return items
                    context->SetStemArrayElement(stem, 0, context->SizeToObject(currentStemIndex));
                    return ERROR_NOMEM;
                }

                // now build the return value
                strncpy(lineBuffer, sizeof(lineBuffer), lineNumber);
                memcpy(lineBuffer + strlen(lineNumber), line, size);

                RexxStringObject returnValue = context->NewString(lineBuffer, totalLineSize);

                free(lineBuffer);

                context->SetStemArrayElement(stem, ++currentStemIndex, returnValue);
                context->ReleaseLocalReference(returnValue);
            }
            else
            {
                RexxStringObject returnValue = context->NewString(line, size);

                context->SetStemArrayElement(stem, ++currentStemIndex, returnValue);
                context->ReleaseLocalReference(returnValue);
            }
        }
    }

    fileSource.close();

    // make sure we update the count with the number of return items
    context->SetStemArrayElement(stem, 0, context->SizeToObject(currentStemIndex));
    return "";                           /* no error on call           */
}


#define INTERNAL_ROUTINE(name, entry) REXX_TYPED_ROUTINE_PROTOTYPE(entry)

#include "SysRexxutilFunctions.h"          // generate prototypes for the system functions.

// now redefine to generate the table entries
#undef  INTERNAL_ROUTINE
#define INTERNAL_ROUTINE(name, entry) REXX_TYPED_ROUTINE(name, entry),


// now build the actual entry list
RexxRoutineEntry rexxutil_routines[] =
{
    REXX_TYPED_ROUTINE(SysAddRexxMacro,        SysAddRexxMacro),
    REXX_TYPED_ROUTINE(SysDropRexxMacro,       SysDropRexxMacro),
    REXX_TYPED_ROUTINE(SysReorderRexxMacro,    SysReorderRexxMacro),
    REXX_TYPED_ROUTINE(SysQueryRexxMacro,      SysQueryRexxMacro),
    REXX_TYPED_ROUTINE(SysClearRexxMacroSpace, SysClearRexxMacroSpace),
    REXX_TYPED_ROUTINE(SysLoadRexxMacroSpace,  SysLoadRexxMacroSpace),
    REXX_TYPED_ROUTINE(SysSaveRexxMacroSpace,  SysSaveRexxMacroSpace),
    REXX_TYPED_ROUTINE(SysDropFuncs,           SysDropFuncs),
    REXX_TYPED_ROUTINE(SysLoadFuncs,           SysLoadFuncs),
    REXX_TYPED_ROUTINE(SysDumpVariables,       SysDumpVariables),
    REXX_TYPED_ROUTINE(SysStemSort,            SysStemSort),
    REXX_TYPED_ROUTINE(SysStemDelete,          SysStemDelete),
    REXX_TYPED_ROUTINE(SysStemInsert,          SysStemInsert),
    REXX_TYPED_ROUTINE(SysStemCopy,            SysStemCopy),
    REXX_TYPED_ROUTINE(SysUtilVersion,         SysUtilVersion),
    REXX_TYPED_ROUTINE(SysFileExists,          SysFileExists),
    REXX_TYPED_ROUTINE(SysFileIsLink,          SysFileIsLink),
#include "SysRexxutilFunctions.hpp"
    REXX_LAST_ROUTINE()
};

RexxPackageEntry rexxutil_package_entry =
{
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "REXXUTIL",                          // name of the package
    "5.0.0",                             // package information
    NULL,                                // no load/unload functions
    NULL,
    rexxutil_routines,                   // the exported functions
    NULL                                 // no methods in this package
};

// and finally plug this in to the package manager.
RexxPackageEntry *PackageManager::rexxutilPackage = &rexxutil_package_entry;
