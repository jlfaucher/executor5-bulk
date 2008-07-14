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

#include "LocalMacroSpaceManager.hpp"
#include "LocalAPIManager.hpp"
#include "InternalAPI.hpp"
#include "SysLibrary.hpp"
#include "ClientMessage.hpp"
#include "SysFile.hpp"
#include "rexxapitypes.h"
#include <stdio.h>


MacroSpaceFile::~MacroSpaceFile()
{
    // we're being terminated with the file still open...delete, and
    // erase the file we have so far.
    if (fileInst != NULL)
    {

        fileInst->close();
        // if we were trying to create this file, erase the
        // partially created one.
        if (creating)
        {
            remove(fileName);
        }
    }
}

void MacroSpaceFile::close()
{
    fileInst->close();
}

size_t MacroSpaceFile::openForLoading()
{
    MacroSpaceFileHeader header;
    bool opened;

    // open the file
    fileInst = new SysFile();
    opened = fileInst->open(fileName, RX_O_BINARY | RX_O_RDONLY, RX_SH_DENYRW, 0);
    if (opened == false)
    {
        throw new ServiceException(FILE_CREATION_ERROR, "Unable to open macrospace file");
    }
    creating = false;                // we're just reading this
    read(&header, sizeof(header));   // read the header information

    if (memcmp(header.version, RXVERSION, RXVERSIZE) != 0)
    {
        throw new ServiceException(MACROSPACE_VERSION_ERROR, "Incompatible macro space version");
    }

    if (header.magicNumber != SIGNATURE)
    {
        throw new ServiceException(MACROSPACE_SIGNATURE_ERROR, "Incompatible macro space signature");
    }
    descriptorBase = sizeof(header);     // now mark the position of the descriptors
                                         // and the calculated start position of the
                                         // image data.
    imageBase = sizeof(MacroSpaceDescriptor) * header.count;

    return header.count;                 // we have a size, return it.
}

void MacroSpaceFile::nextMacro(char *name, ManagedRxstring &image, size_t &order)
{
    setFilePosition(descriptorBase);
    descriptorBase += sizeof(MacroSpaceDescriptor);
    MacroSpaceDescriptor desc;

    read(&desc, sizeof(desc));
    strcpy(name, desc.name);
    order = desc.position;
    setFilePosition(imageBase);
    imageBase += desc.imageSize;
    read(image, desc.imageSize);
}

void MacroSpaceFile::nextMacro(NameTable names, char *name, ManagedRxstring &image, size_t &order)
{
    setFilePosition(descriptorBase);
    descriptorBase += sizeof(MacroSpaceDescriptor);
    MacroSpaceDescriptor desc;

    read(&desc, sizeof(desc));
    descriptorBase += sizeof(desc);

    // we only read the image data in if this is in the requested list
    if (names.inTable(desc.name))
    {
        strcpy(name, desc.name);
        order = desc.position;
        setFilePosition(imageBase);
        imageBase += desc.imageSize;
        read(image, desc.imageSize);
    }
    else
    {
        // even though not reading this, we need to update the read position
        imageBase += desc.imageSize;

    }
}

void MacroSpaceFile::setFilePosition(int p)
{
    int64_t position;
    if (fileInst->seek(p, SEEK_CUR, position) == false)
    {
        throw new ServiceException(FILE_READ_ERROR, "Error reading from macrospace file");
    }
}


void MacroSpaceFile::create(size_t count)
{
    bool opened;
    // create the file
    fileInst = new SysFile;
    opened = fileInst->open(fileName, RX_O_CREAT | RX_O_BINARY | RX_O_TRUNC | RX_O_WRONLY,  RX_SH_DENYRW, RX_S_IREAD | RX_S_IWRITE);

    if (opened == false)
    {
        throw new ServiceException(FILE_CREATION_ERROR, "Unable to create macrospace file");
    }
    creating = true;

    MacroSpaceFileHeader header(count);
    write(&header, sizeof(header));
}

void MacroSpaceFile::writeMacroDescriptor(char *name, int size, size_t order)
{
    MacroSpaceDescriptor desc(name, size, order);

    write(&desc, sizeof(desc));
}

void MacroSpaceFile::write(const void *data, int length)
{
    size_t bytesWritten;
    fileInst->write((const char *)data, length, bytesWritten);
    if (bytesWritten != (size_t)length)
    {
        throw new ServiceException(FILE_WRITE_ERROR, "Error writing to macrospace file");
    }
}

void MacroSpaceFile::read(void *data, int length)
{
    size_t bytesRead;
    fileInst->read((char *)data, length, bytesRead);
    if (bytesRead != (size_t)length)
    {
        throw new ServiceException(FILE_READ_ERROR, "Error reading from macrospace file");
    }
}

void MacroSpaceFile::read(ManagedRxstring &data, int length)
{
    data.ensureCapacity(length);
    read(data.strptr, length);
    data.strlength = length;
}

void LocalMacroSpaceManager::loadMacroSpace(char *target)
{
    // now open and read the file header
    MacroSpaceFile file(target);
    // validate the file
    size_t count = file.openForLoading();

    ManagedRxstring image;    // this is outside the loop, which gives us the chance to reuse the buffer

    for (size_t i = 0; i < count; i++)
    {
        char macroName[MacroSpaceDescriptor::MACRONAMESIZE];
        size_t order;

        file.nextMacro(macroName, image, order);

        ClientMessage message(MacroSpaceManager, ADD_MACRO, macroName);
        message.parameter1 = image.strlength;
        message.parameter2 = order;

        // request the next one.
        message.send();
    }
    file.close();
}

void LocalMacroSpaceManager::loadMacroSpace(char *target, char **nameList, size_t nameCount)
{
    NameTable names(nameList, nameCount);

    // now open and read the file header
    MacroSpaceFile file(target);
    // validate the file
    size_t count = file.openForLoading();

    ManagedRxstring image;    // this is outside the loop, which gives us the chance to reuse the buffer

    for (size_t i = 0; i < count; i++)
    {
        char macroName[MacroSpaceDescriptor::MACRONAMESIZE];
        size_t order;

        file.nextMacro(names, macroName, image, order);

        ClientMessage message(MacroSpaceManager, ADD_MACRO, macroName);
        message.parameter1 = image.strlength;
        message.parameter2 = order;

        // request the next one.
        message.send();
    }
    file.close();
}


void LocalMacroSpaceManager::saveMacroSpace(char *target)
{
    ClientMessage message(MacroSpaceManager, ITERATE_MACRO_DESCRIPTORS);

    message.send();

    // we're empty, no point in this.
    if (message.parameter1 == 0)
    {
        return;
    }

    // now open and write the file header
    MacroSpaceFile file(target);
    file.create(message.parameter1);
    message.operation = NEXT_MACRO_DESCRIPTOR;

    for (;;)
    {
        // request the next one.
        message.send();
        if (message.result == NO_MORE_MACROS)
        {
            break;
        }
        file.writeMacroDescriptor(message.nameArg, message.parameter1, message.parameter2);
    }
    // now iterate the images
    message.operation = ITERATE_MACROS;
    message.send();

    message.operation = NEXT_MACRO_IMAGE;
    for (;;)
    {
        // request the next one.
        message.send();
        if (message.result == NO_MORE_MACROS)
        {
            break;
        }
        file.write(message.getMessageData(), message.getMessageDataLength());
        message.freeMessageData();
    }
    // all done!
    file.close();
}


void LocalMacroSpaceManager::getMacro(char *target, RxString *image)
{
    ClientMessage message(MacroSpaceManager, GET_MACRO_IMAGE, target);

    // request, then receive the image data
    message.send();
    MakeRxString(*image, message.getMessageData(), message.getMessageDataLength());
}

void LocalMacroSpaceManager::saveMacroSpace(char *target, char **names, size_t count)
{
    // now open and write the file header
    MacroSpaceFile file(target);
    file.create(count);
    size_t i;

    ClientMessage message(MacroSpaceManager, GET_MACRO_DESCRIPTOR);

    for (i = 0; i < count; i++)
    {
        strcpy(message.nameArg, names[i]);
        // request the next one.  This will throw an exception if it doesn't exist.
        message.send();
        file.writeMacroDescriptor(message.nameArg, message.parameter1, message.parameter2);
    }
    // now iterate the images
    message.operation = GET_MACRO_IMAGE;

    for (i = 0; i < count; i++)
    {
        strcpy(message.nameArg, names[i]);
        // request the next one.  This will throw an exception if it doesn't exist.
        message.send();
        file.write(message.getMessageData(), message.getMessageDataLength());
    }
    // all done!
    file.close();
}

void LocalMacroSpaceManager::clearMacroSpace()
{
    ClientMessage message(MacroSpaceManager, CLEAR_MACRO_SPACE);
    message.send();
}

void LocalMacroSpaceManager::removeMacro(char *name)
{
    ClientMessage message(MacroSpaceManager, REMOVE_MACRO, name);
    message.send();
}

void LocalMacroSpaceManager::queryMacro(char *name, size_t *pos)
{
    ClientMessage message(MacroSpaceManager, QUERY_MACRO, name);
    message.send();
    *pos = message.parameter1;
}

void LocalMacroSpaceManager::reorderMacro(char *name, size_t pos)
{
    ClientMessage message(MacroSpaceManager, REORDER_MACRO, name);
    message.parameter1 = pos;
    message.send();
}

void LocalMacroSpaceManager::addMacroFromFile(char *name, char *sourceFile, size_t position)
{
    ManagedRxstring imageData;

    // translate the image
    translateRexxProgram(sourceFile, imageData);
    addMacro(name, imageData, position);
}

void LocalMacroSpaceManager::addMacro(char *name, ManagedRxstring &imageData, size_t position)
{
    ClientMessage message(MacroSpaceManager, ADD_MACRO, name);
    message.setMessageData(imageData.strptr, imageData.strlength);
                                           // set the additional arguments
    message.parameter1 = imageData.strlength;
    message.parameter2 = position;     // make sure we have the add order

    // attach the image data
    message.setMessageData(imageData.strptr, imageData.strlength);

    message.send();
}

void LocalMacroSpaceManager::translateRexxProgram(char *sourceFile, ManagedRxstring &imageData)
{
    bool opened;

    SysFile *fileInst = new SysFile;
    opened = fileInst->open(sourceFile, RX_O_BINARY | RX_O_RDONLY, RX_SH_DENYWR, 0);
    if (opened == false)
    {
        throw new ServiceException(MACRO_SOURCE_NOT_FOUND, "Unable to open macrospace source file");
    }

    int64_t fsize;
    if (fileInst->getSize(fsize) == false)
    {
        throw new ServiceException(MACRO_SOURCE_READ_ERROR, "Unable to read macrospace source file");
    }

    // we define imageData outside this block and sourceData inside.  Once we've
    // translated the file, we're finished with the source, so it can be released
    // once we exit the block.
    {
        SysLibrary lib;
        if (!lib.load("rexx"))
        {
            throw new ServiceException(MACRO_TRANSLATION_ERROR, "Unable to compile Rexx program");
        }

        void *proc = lib.getProcedure("ooRexxTranslateSource");
        if (proc == NULL)
        {
            throw new ServiceException(MACRO_TRANSLATION_ERROR, "Unable to compile Rexx program");
        }

        RexxReturnCode (RexxEntry *compiler)(RexxStringPointer, RxString *, RxString *);

        compiler = (RexxReturnCode (RexxEntry *)(RexxStringPointer, RxString *, RxString *))proc;

        ManagedRxstring sourceData;
        readRxstringFromFile(fileInst, sourceData, fsize);
        fileInst->close();
        imageData.strptr = NULL;
        imageData.strlength = 0;

        RexxReturnCode rc = (*compiler)(sourceFile, (RxString *)&sourceData, (RxString *)&imageData);
        if (rc != 0)
        {
            throw new ServiceException(MACRO_TRANSLATION_ERROR, "Unable to compile Rexx program");
        }
    }
}

void LocalMacroSpaceManager::readRxstringFromFile(SysFile * fileInst, ManagedRxstring &target, size_t size)
{
    size_t bytesRead;

    target.strlength = size;
    if (size > 0)                          // if bytes to read           */
    {
        target.ensureCapacity(size);

        fileInst->read(target.strptr, size, bytesRead);
        if (bytesRead != size)
        {
            throw new ServiceException(MACROSPACE_FILE_READ_ERROR, "Unable to read macro space file");
        }
    }
}


RexxReturnCode LocalMacroSpaceManager::processServiceException(ServiceException *e)
{
    switch (e->getErrorCode())
    {
        case MACRO_SOURCE_NOT_FOUND:
        case MACRO_TRANSLATION_ERROR:
            return RXMACRO_SOURCE_NOT_FOUND;

        case MACRO_SOURCE_READ_ERROR:
        case FILE_CREATION_ERROR:
        case MACROSPACE_FILE_READ_ERROR:
        case FILE_READ_ERROR:
        case FILE_WRITE_ERROR:
            return RXMACRO_FILE_ERROR;

        case MACROSPACE_VERSION_ERROR:
        case MACROSPACE_SIGNATURE_ERROR:
            return RXMACRO_SIGNATURE_ERROR;

        case MACRO_DOES_NOT_EXIST:
            return RXMACRO_NOT_FOUND;

        case MACRO_LOAD_REXX:
        default:
            return RXMACRO_NO_STORAGE;
    }
}
