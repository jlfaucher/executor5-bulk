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
/*********************************************************************/
/*                                                                   */
/* Unflatten saved methods from various sources.                     */
/*                                                                   */
/*********************************************************************/

#include "RexxCore.h"
#include "StringClass.hpp"
#include "RexxBuffer.hpp"
#include "RexxSmartBuffer.hpp"
#include "MethodClass.hpp"
#include "RexxCode.hpp"
#include "RexxActivity.hpp"
#include "SourceFile.hpp"
#include "oorexx.h"
#include "Interpreter.hpp"
#include "SysInterpreter.hpp"
#include "InternalApi.hpp"

/*********************************************************************/
/*         Definitions for use by the Meta I/O functionality         */
/*********************************************************************/

#define  METAVERSION       35
                                       /* to ::class instruction errors     */
                                       /* 29 => 31 bump for change in hash  */
                                       /* algorithm                         */
                                       /* 31 => 34 bump for new kernel and  */
                                       /* USERID BIF                        */
#define  MAGIC          11111          /* function                          */

const char compiledHeader[] = "/**/@REXX";


/*********************************************************************/
/*                                                                   */
/*  Function:    restoreProgram                                      */
/*                                                                   */
/*  Description: This function is used to load the meta data from a  */
/*               file into the proper interpreter variables.         */
/*                                                                   */
/*********************************************************************/

void RexxMethod::restoreProgram(
  RexxString *fileName,                /* name of file to process           */
  ProtectedObject &method)             // returned method

{
  FILE         *handle;                /* output file handle                */
  char         *file;                  /* ASCII-Z file name                 */

  // default is nothing saved
  method = OREF_NULL;

  if (RexxMemory::memoryObject.savingImage())    /* doing save image?                 */
  {
      return;                          /* never restore during image build  */
  }

  file = (char *)fileName->getStringData(); /* get the file name pointer         */

  handle = fopen(file, "rb");          /* open the input file               */
  if (handle == NULL)                  /* get an open error?                */
  {
      return;                            /* no restored image                 */
  }
                                       /* see if this is a "sourceless" one */
  method = restoreTranslatedProgram(fileName, handle);
  if ((RexxObject *)method != OREF_NULL)             /* get a method out of this?         */
  {
      return;                            /* this process is finished          */
  }

  method = restoreSavedTranslatedProgram(fileName, handle);

  fclose(handle);                    /*cclose the file                    */

  if ((RexxObject *)method == OREF_NULL)           // non-restorable?
  {
      // report the error
      reportException(Error_Program_unreadable_version, fileName);
  }
}

/*********************************************************************/
/*                                                                   */
/*  Function:    SysSaveProgram                                      */
/*                                                                   */
/*  Description: This function saves a flattened method to a file    */
/*                                                                   */
/*********************************************************************/
void RexxMethod::saveProgram(
  RexxString * fileName)               /* name of file to process           */
{
  FILE          *handle;                /* file handle                      */
  FILE_CONTROL  control;               /* control information               */
  RexxBuffer *  methodBuffer;          /* flattened method                  */
  RexxSmartBuffer *flatBuffer;         /* fully flattened buffer            */
  char         *bufferAddress;         /* address of flattened method data  */
  size_t        bufferLength;          /* length of the flattened method    */
  char         *file;                  /* ASCII-Z file name                 */

  if (RexxMemory::memoryObject.savingImage())    /* doing save image?                 */
  {
      return;                          /* never save during image build     */
  }

  // only save if the interpreter says it is allowed
  if (!SysInterpreter::savingPrograms())
  {
      return;
  }

  ProtectedObject p1(fileName);             // protect the info during the save process
  ProtectedObject p2(this);
  file = (char *)fileName->getStringData();         /* get the file name pointer         */

  handle = fopen(file, "a+b");         /* open the output file  */
  if (handle == NULL)                  /* get an open error?                */
  {
      return;
  }

  flatBuffer = saveMethod();           /* flatten the method                */
  ProtectedObject p3(flatBuffer);
                                       /* retrieve the length of the buffer */
  bufferLength = flatBuffer->current;
  methodBuffer = flatBuffer->buffer;   /* get to the actual data buffer     */
  bufferAddress = (char *)methodBuffer->address();  /* retrieve buffer starting address  */
  // fill in the control information
  createMetaDescriptor(&control, bufferLength);

  {
      // NO kernel access this section
      UnsafeBlock ublock;
                                           /* write out the REXX signature      */
      putc(0x1a, handle);                  // add a ctrl-Z at the file end
      fwrite(bufferAddress, 1, bufferLength, handle);
                                           // write the control information
      fwrite(&control, 1, sizeof(control), handle);
                                           // and finally the "compiled" tag
      fwrite(compiledHeader, 1, sizeof(compiledHeader), handle);
                                           /* now the control info              */
                                           /* and finally the flattened method  */
      fclose(handle);                      /* done saving                       */
  }
}


/*********************************************************************/
/*                                                                   */
/*  Function:    restoreProgramBuffer                                */
/*                                                                   */
/*  Description: This function is used to unflatten a REXX method    */
/*               from a buffer into a method.                        */
/*                                                                   */
/*********************************************************************/

RexxMethod *RexxMethod::restoreProgramBuffer(
  RxString   * inBuffer,               /* pointer to stored method          */
  RexxString * name)                   /* name associated with the program  */
{
  FILE_CONTROL *control;               /* control information               */
  char         *startPointer;          /* start of buffered information     */
  char         *methodData;            // point to the method data
  RexxBuffer  * buffer;                /* Buffer to unflatten               */
  size_t        bufferSize;            /* size of the buffer                */
  RexxMethod  * method;                /* unflattened method                */
  RexxCode    * code;                  /* parent rexx method                */
  RexxSource  * source;                /* REXX source object                */

                                       /* address the control information   */
  control = (FILE_CONTROL*)inBuffer->strptr;
                                       /* check the control info            */
  if ((control->metaVersion != METAVERSION) || (control->magic != MAGIC))
  {
    return OREF_NULL;                  /* can't load it                     */
  }
  methodData = inBuffer->strptr + sizeof(FILE_CONTROL);
                                       /* get the buffer size               */
  bufferSize = inBuffer->strlength - sizeof(FILE_CONTROL);


  // we need to copy this into a buffer object because the restoration process
  // will create the method objects in place, and resize the containing
  // buffer object to release them into the object space
  buffer = new_buffer(bufferSize);     /* get a new buffer                  */
  ProtectedObject p1(buffer);
                                       /* position relative to the end      */
  startPointer = (char *)buffer->address();
                                       /* fill in the buffer                */
  memcpy(startPointer, methodData, bufferSize);
                                       /* "puff" this out usable form       */
  method = RexxMethod::restore(buffer, (stringchar_t *)startPointer);
  code = (RexxCode *)method->getCode();/* get the REXX code object          */
  source = code->u_source;             /* and now the source object         */
                                       /* switch the file name (this might  */
                                       /* be different than the name        */
  source->setProgramName(name);        /* originally saved under            */
                                       /* NOTE:  no source file reconnect   */
                                       /* is possible here                  */
  return method;                       /* return the unflattened method     */
}


                                       /* point to the flattened method     */
/*********************************************************************/
/*                                                                   */
/*  Function:    SysRestoreInstoreProgram                            */
/*                                                                   */
/*  Description: This function is used to load the meta data from an */
/*               instorage buffer.                                   */
/*                                                                   */
/*********************************************************************/
RexxMethod *RexxMethod::restoreInstoreProgram(
  RxString    *inBuffer,               /* pointer to stored method          */
  RexxString * name)                   /* name associated with the program  */
{
  RxString     imageData;              /* actual image part of this         */

                                       /* not a compiled file?              */
  if (strcmp(inBuffer->strptr, compiledHeader) != 0)
  {
      return OREF_NULL;                  /* not a saved program               */
  }
                                       /* point to the image start          */
  imageData.strptr = inBuffer->strptr + sizeof(compiledHeader);
                                       /* and adjust the length too         */
  imageData.strlength = inBuffer->strlength - sizeof(compiledHeader);
                                       /* now go unflatten this             */
  return restoreProgramBuffer(&imageData, name);
}

/*********************************************************************/
/*                                                                   */
/*  Function:    saveProgramBuffer                                   */
/*                                                                   */
/*  Description: This function saves a flattened method to an        */
/*               RxString buffer                                     */
/*                                                                   */
/*********************************************************************/
void RexxMethod::saveProgramBuffer(
  RxString    *outBuffer)              /* location to save the program      */
{
  FILE_CONTROL *control;               /* control information               */
  char         *buffer;                /* buffer pointer                    */
  RexxBuffer  * methodBuffer;          /* flattened method                  */
  RexxSmartBuffer *flatBuffer;         /* flattened smart buffer            */
  char        * bufferAddress;         /* address of flattened method data  */
  size_t        bufferLength;          /* length of the flattened method    */

  ProtectedObject p1(this);            // make sure we're protected

  flatBuffer = this->saveMethod();     /* flatten the method                */
                                       /* retrieve the length of the buffer */
  bufferLength = flatBuffer->current;
  methodBuffer = flatBuffer->buffer;   /* get to the actual data buffer     */
  bufferAddress = (char *)methodBuffer->address();  /* retrieve buffer starting address  */
                                       /* get the final buffer              */
  buffer = (char *)SysInterpreter::allocateResultMemory(bufferLength + sizeof(FILE_CONTROL));
  outBuffer->strptr = buffer;          /* fill in the result pointer        */
                                       /* and the result length             */
  outBuffer->strlength = bufferLength + sizeof(FILE_CONTROL);

  control = (FILE_CONTROL *)buffer;    /* set pointer to control info       */
  // fill in the control information
  createMetaDescriptor(control, bufferLength);
  buffer = buffer + sizeof(FILE_CONTROL);  /* step the buffer pointer           */
                                       /* Copy the method buffer            */
  memcpy(buffer, bufferAddress, bufferLength);
}



/*********************************************************************/
/*                                                                   */
/*  Function:    createMetaDescriptor                                */
/*                                                                   */
/*  Description: fill in the metadata for a saved image.             */
/*                                                                   */
/*********************************************************************/
void RexxMethod::createMetaDescriptor(FILE_CONTROL *control, size_t bufferLength)
{
                                         /* clear out the cntrol info         */
    memset((void *)control, 0, sizeof(FILE_CONTROL));
                                         /* fill in version info              */
    strcpy(control->rexxVersion, SysInterpreter::metaVersion);
    RexxString *version = RexxInterpreter::versionString(); /* get the version string            */
    rstrcat(control->rexxVersion, version);
    control->metaVersion = METAVERSION;   /* current meta version              */
    control->magic = MAGIC;               /* magic signature number            */
    control->imageSize = bufferLength;    /* add the buffer length             */
}


/*********************************************************************/
/*                                                                   */
/*  Function:    saveTranslatedProgram                               */
/*                                                                   */
/*  Description: This function saves a flattened method to a file    */
/*                                                                   */
/*********************************************************************/

void RexxMethod::saveTranslatedProgram(
  stringchar_t *file)                  /* name of file to process           */
{
  FILE         *handle;                /* output file handle                */
  FILE_CONTROL  control;               /* control information               */
  RexxBuffer *  methodBuffer;          /* flattened method                  */
  RexxSmartBuffer *flatBuffer;         /* flattened smart buffer            */
  char         *bufferAddress;         /* address of flattened method data  */
  size_t        bufferLength;          /* length of the flattened method    */

  handle = fopen((char *)file, "wb");  /* open the output file              */
  if (handle == NULL)                  /* get an open error?                */
  {
                                       /* got an error here                 */
      reportException(Error_Program_unreadable_output_error, new_string(file));
  }
  ProtectedObject p1(this);            // make sure we're protected
  flatBuffer = saveMethod();           // flatten the method
  ProtectedObject p2(flatBuffer);
                                       /* retrieve the length of the buffer */
  bufferLength = flatBuffer->current;
  methodBuffer = flatBuffer->buffer;   /* get to the actual data buffer     */
  bufferAddress = (char *)methodBuffer->address();  /* retrieve buffer starting address  */
  // fill in the control information
  createMetaDescriptor(&control, bufferLength);

  {
      // NO kernel access this section
      UnsafeBlock ublock;
                                           /* write out the REXX signature      */
      fwrite(compiledHeader, 1, sizeof(compiledHeader), handle);
                                           /* now the control info              */
      fwrite(&control, 1, sizeof(control), handle);
                                           /* and finally the flattened method  */
      fwrite(bufferAddress, 1, bufferLength, handle);
      fclose(handle);                      /* done saving                       */
  }
}


/*********************************************************************/
/*                                                                   */
/*  Function:    restoreSavedTranslatedProgram                       */
/*                                                                   */
/*  Description: This function is used to load a flattened method    */
/*               from a file into the proper interpreter variables.  */
/*                                                                   */
/*********************************************************************/

RexxMethod *RexxMethod::restoreSavedTranslatedProgram(
  RexxString *fileName,                /* name of file to process           */
  FILE       *handle )                 /* handle of the file to process     */
{
    FILE_CONTROL  control;               /* control information               */
    char        * startPointer;          /* start of buffered method          */
    RexxBuffer  * buffer;                /* Buffer to unflatten               */
    size_t        bufferSize;            /* size of the buffer                */
    size_t        bytesRead = 0;         /* actual bytes read                 */
    RexxMethod  * method;                /* unflattened method                */
    RexxCode    * code;                  /* parent rexx method                */
    RexxSource  * source;                /* REXX source object                */
                                         /* temporary read buffer             */
    char          fileTag[sizeof(compiledHeader)];
    {
        UnsafeBlock ublock;
        // seek to the expected location of the header and read it, if possible
        if (fseek(handle, 0-sizeof(compiledHeader), SEEK_END) != 0)
        {
            // return a failure indicator
            return OREF_NULL;
        }
        bytesRead = fread(fileTag, 1 ,sizeof(compiledHeader), handle);
        // we should see the compiled tag here
        if ((bytesRead != sizeof(compiledHeader)) || (strcmp(fileTag, compiledHeader) != 0))
        {
            return OREF_NULL;                  /* not a saved program               */
        }
        // back up and read the control information
        if (fseek(handle, 0 - sizeof(compiledHeader) - sizeof(control), SEEK_END) != 0)
        {
            // return a failure indicator
            return OREF_NULL;
        }
        // now read the control information
        bytesRead = fread((char *)&control, 1, sizeof(control), handle);
    }

                                         /* check the control info            */
    if ((bytesRead != sizeof(control)) || (control.metaVersion != METAVERSION) || (control.magic != MAGIC))
    {
        // return a failure indicator
        return OREF_NULL;
    }
                                             /* read the file size                */
    bufferSize = control.imageSize;          /* get the method info size          */

    buffer = new_buffer(bufferSize);
    ProtectedObject p1(buffer);              // protect this from GC.

    startPointer = (char *)buffer->address();// point to the buffer location
    {
        UnsafeBlock ublock;
        // move to the image location
        if (fseek(handle, 0-sizeof(compiledHeader)-sizeof(FILE_CONTROL)-bufferSize, SEEK_END) != 0)
        {
            return OREF_NULL;                  /* not a saved program               */
        }

        // read the tokenized program directly into the buffer
        bytesRead = fread(startPointer, 1, bufferSize, handle);
    }

                                         /* "puff" this out usable form       */
    method = RexxMethod::restore(buffer, (stringchar_t *)startPointer);
    ProtectedObject p2(method);

    code = (RexxCode *)method->code;     /* get the REXX code object          */
    source = code->u_source;             /* and now the source object         */
                                         /* switch the file name (this might  */
                                         /* be different than the name        */
    source->setProgramName(fileName);    /* originally saved under            */
    source->setReconnect();              /* allow source file reconnection    */
    return method;                       /* return the unflattened method     */
}


/*********************************************************************/
/*                                                                   */
/*  Function:    restoreTranslatedProgram                            */
/*                                                                   */
/*  Description: This function is used to load a flattened method    */
/*               from a file into the proper interpreter variables.  */
/*                                                                   */
/*********************************************************************/

RexxMethod *RexxMethod::restoreTranslatedProgram(
  RexxString *fileName,                /* name of file to process           */
  FILE       *handle )                 /* handle of the file to process     */
{
  FILE_CONTROL  control;               /* control information               */
  char        * startPointer;          /* start of buffered method          */
  RexxBuffer  * buffer;                /* Buffer to unflatten               */
  size_t        bufferSize;            /* size of the buffer                */
  size_t        bytesRead;             /* actual bytes read                 */
                                       /* temporary read buffer             */
  char          fileTag[sizeof(compiledHeader)];

  {
      // NO kernel access this section
      UnsafeBlock ublock;
                                           /* read the first file part          */
      bytesRead = fread(fileTag, 1, sizeof(compiledHeader), handle);
                                           /* not a compiled file?              */
      if (strcmp(fileTag, compiledHeader) != 0)
      {
        fclose(handle);                    /* close the file                    */
        return OREF_NULL;                  /* not a saved program               */
      }
                                           /* now read the control info         */
      bytesRead = fread((char *)&control, 1, sizeof(control), handle);
  }
                                       /* check the control info            */
  if ((control.metaVersion != METAVERSION) || (control.magic != MAGIC))
  {
      return OREF_NULL;                // not really translated, try to process as source
  }
                                       /* read the file size                */
  bufferSize = control.imageSize;      /* get the method info size          */
  buffer = new_buffer(bufferSize);     /* get a new buffer                  */
  ProtectedObject p1(buffer);
                                       /* position relative to the end      */
  startPointer = ((char *)buffer + ObjectSize(buffer)) - bufferSize;
  {
      // NO kernel access this section
      UnsafeBlock ublock;
                                           /* read the flattened method         */
      bytesRead = fread(startPointer, 1, bufferSize, handle);
      fclose(handle);                      /* close the file                    */
  }
                                       /* "puff" this out usable form       */
  RexxMethod *method = RexxMethod::restore(buffer, (stringchar_t *)startPointer);
  ProtectedObject p2(method);
                        /* buffer need not to be holded because it is now an envelope and referenced by Method */
  RexxCode *code = (RexxCode *)method->getCode();     /* get the REXX code object          */
  RexxSource *source = code->u_source; /* and now the source object         */
                                       /* switch the file name (this might  */
                                       /* be different than the name        */
  source->setProgramName(fileName);    /* originally saved under            */
  return method;                       /* return the unflattened method     */
}

/*********************************************************************/
/*                                                                   */
/*  Function:    getMacroCode                                        */
/*                                                                   */
/*  Description: Retrieve a method from the macro space              */
/*                                                                   */
/*********************************************************************/
RexxMethod *RexxMethod::getMacroCode(
  RexxString *name)                    /* name of file to process           */
{
    RxString buffer;                   // retrieved image buffer

    if (ooRexxResolveMacroFunction((RexxStringPointer)name->getStringData(), &buffer) == RXMACRO_OK)
    {
        RexxMethod *method = restoreProgramBuffer(&buffer, name);
        SysInterpreter::releaseResultMemory(buffer.strptr);
        return method;
    }
    return OREF_NULL;
}

void RexxMethod::translateSource(
   stringchar_t         * inputName,   /* input program name                */
   stringchar_t         * outputName ) /* output file name                  */
/******************************************************************************/
/* Function:  Process instorage execution arguments                           */
/******************************************************************************/
{
  RexxString * fullName;               /* fully resolved input name         */
  RexxString * inName = new_string(inputName);
  ProtectedObject p0(inName);

  // kernel access released in this section
  {
      UnsafeBlock ublock;
      fullName = SysInterpreter::resolveProgramName(inName, OREF_NULL);
  }
  // end of released access point

  if (fullName == NULL)
  {
                                         /* got an error here                 */
      reportException(Error_Program_unreadable_notfound, inName);
  }
  ProtectedObject p1(fullName);
                                       /* go translate the image            */
  RexxMethod *method = RexxMethod::newFile(fullName);

  ProtectedObject p2(method);
  if (outputName != NULL)              /* want to save this to a file?      */
  {
                                       /* go save this method               */
      method->saveTranslatedProgram(outputName);
  }
}


/**
 * Translate an instore source program into an instore image.
 *
 * @param name   The program name.
 * @param input  Buffered input source.
 * @param output Translated output image.
 */
void RexxMethod::translateSource(stringchar_t *name, RxString *input, RxString *output)
{
    processInstoreExecution(input, output, new_string(name));
}

RexxMethod * RexxMethod::processInstoreExecution(
   RxString   *instoreSource,          // instore source buffer
   RxString   *instoreImage,           // a translated instore image
   RexxString * name)                  // program name
/******************************************************************************/
/* Function:  Process instorage execution arguments                           */
/******************************************************************************/
{
  // we might be executing directly from an already translated image
  if (instoreImage != NULL && instoreImage->strptr != NULL)
  {
                                       /* go convert into a method          */
    RexxMethod *method = restoreProgramBuffer(instoreImage, name);

    ProtectedObject p1(method);        // protect the object
    if (method != OREF_NULL)           /* did it unflatten successfully?    */
    {
        // if the source was provided with the image, then we can reconnect the
        // two for tracing/debug
      if (instoreSource != NULL && instoreSource->strptr != NULL)
      {
        // source is stored in buffer form, so allocate and copy the source data
        RexxBuffer *source_buffer = new_buffer(instoreSource->strlength);
        source_buffer->copyData(0, (stringchar_t *)instoreSource->strptr, instoreSource->strlength);
                                       /* reconnect this with the source    */
        method->getRexxCode()->getSource()->setBufferedSource(source_buffer);
      }
      return method;                   /* go return it                      */
    }
  }

  // unable to restore from provided image, try to execute from source
  // first check to see if the source was given to us instore
  if (instoreSource != NULL && instoreSource->strptr != NULL)
  {
                                       /* check for a compiled program      */
    RexxMethod *method = restoreInstoreProgram(instoreSource, name);
    if (method == OREF_NULL) {         /* not precompiled?                  */
                                       /* get a buffer object               */
      RexxBuffer *buffer = new_buffer(instoreSource->strlength);
      ProtectedObject p2(buffer);        // protect the object
                                       /* copy source into the buffer       */
      buffer->copyData(0, (stringchar_t *)instoreSource->strptr, instoreSource->strlength);
                                       /* translate this source             */
      method = RexxMethod::newRexxBuffer(name, buffer);
                                       /* return this back in instore[1]    */
      method->saveProgramBuffer(instoreImage);
    }
    return method;                     /* return translated source          */
  }
  return OREF_NULL;                    /* processing failed                 */
}
