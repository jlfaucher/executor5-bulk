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

#ifndef Included_RexxUtilCommon
#define Included_RexxUtilCommon

#include "SysFileSystem.hpp"
#include "FileNameBuffer.hpp"


/*********************************************************************/
/* Numeric Error Return Strings                                      */
/*********************************************************************/

#define  ERROR_NOMEM      "2"          /* Insufficient memory        */

/*********************************************************************/
/* Alpha Numeric Return Strings                                      */
/*********************************************************************/

#define  ERROR_RETSTR   "ERROR:"

/*********************************************************************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/*********************************************************************/

void inline outOfMemoryException(RexxCallContext *c)
{
    c->ThrowException1(Rexx_Error_System_service_user_defined, c->String("failed to allocate memory"));
}

/**
 * <routineName> argument <argPos> must not be a null string
 *
 * SysFileTree argument 2 must not be a null string
 *
 * @param c      Threade context we are operating in.
 * @param fName  Routine name.
 * @param pos    Argument position.
 */
void inline nullStringException(RexxCallContext *c, const char *fName, size_t pos)
{
    c->ThrowException2(Rexx_Error_Incorrect_call_null, c->String(fName), c->StringSize(pos));
}

/*
 *  Class to perform SysFileTree functions
 *
 *  Note that in Windows the MAX_PATH define includes the terminating null.
 */
class TreeFinder
{
     typedef enum
     {
         RECURSE,        // recursive search
         DO_DIRS,        // return directories
         DO_FILES,       // return files
         NAME_ONLY,      // only return the names
         EDITABLE_TIME,  // return the time in "editable" format
         LONG_TIME,      // return the long time
         CASELESS,       // perform a caseless search
         LONG_SIZE,      // return sizes greater than 4Gb
     } OptionFlags;

     class AttributeMask
     {
      public:

          typedef enum
          {
              Archive,
              Directory,
              Hidden,
              ReadOnly,
              System,
              Control,
          } AttributeType;

          AttributeMask()
          {
              memset(mask, 0, sizeof(mask));
          }

          bool parseMask(const char *maskArg)
          {
              // everything is off until a + or - is found in the mask
              mask[Control] = IgnoreAll;
              int y = 0;

              while (*maskArg)
              {
                  select(*maskArg)
                  {
                      case '+':
                  {
                      mask[y] = AttributeOn;
                      mask[Control] = HaveMask;
                      break;
                  }
                      case '-':
                  {
                      mask[y] = AttributeOff;
                      mask[Control] = HaveMask;
                      break;
                  }
                      case '*':
                  {
                      mask[y] = AttributeIgnore;
                      break;
                  }
                      default:
                      return false;
                  }

                  y++;
                  maskArg++;
              }
              return true;
          }

          bool noNewSettings() { return mask[Control] == IgnoreAll; }
          bool isOn(AttributeType maskSetting) { return mask[maskSetting] == AttributeOn; }
          bool isOff(AttributeType maskSetting) { return mask[maskSetting] == AttributeOff; }
          bool isIgnored(AttributeType maskSetting) { return mask[maskSetting] == AttributeIgnore; }
          void set(AttributeType maskSetting, bool on)  { mask[maskSetting] = on ? AttributeOn : AttributeOff; }
          char mask(AttributeType maskSetting) { return mask[maskSetting] == AttributeOn ? maskChars[maskSetting] : '-'; }
          bool isSelected(AttributeType maskSetting, bool value) { return value ? !isOff(maskSetting) : !isOn(maskSetting); }
          bool acceptAll() { return mask[Control] == IgnoreAll; }

      protected:

          const char AttributeOff =  -1;
          const char AttributeOn = 1;
          const char AttributeIgnore = 0;
          const char IgnoreAll = 0;
          const char HaveMask = 1;
          const char maskChars[6] = "ADHRS";

          int8_t mask[8];      // our map for matching
     };


     TreeFinder(RexxCallContext *c, const char *f, RexxStemObject s, const char *opts, const char *targetAttr, const char *newAttr);

     void validateFileSpec();
     void adjustDirectory();
     bool validateFileSpecChars();
     void parseMask(const char *mask, AttributeMask &flags, size_t argPos);
     void badSFTOptsException(const char *actual);
     void badMaskException(size_t pos, const char *actual);
     void getOptions(const char *opts);
     bool goodOpts(const char *opts);
     void formatFile(const char *fileName);
     void getPath();
     void expandNonPath2fullPath();
     void expandPath2fullPath(size_t lastSlashPos);
     void adjustFileSpec();
     void recursiveFindFile(FileNameBuffer &path);



     bool nameOnly() { return (options & NAME_ONLY) != 0; }
     bool longTime() { return (options & LONG_TIME) != 0; }
     bool editableTime() { return (options & EDITABLE_TIME) != 0; }
     bool longSize() { return (options & LONG_SIZE) != 0; }

     RexxCallContext *context;                     // the initial call context
     size_t         count;                         // Number of found file lines
     RexxStemObject files;                         // Stem that holds results.
     RoutineFileNameBuffer filePath;               // the file path portion of the search. Will get updated with recursions.
     RoutineFileNameBuffer fileSpec;               // File name portion of the search for file spec, may contain glob characters.
     RoutineFileNameBuffer foundFile;              // Full path name of found file
     RoutineFileNameBuffer foundFileLine;          // Buffer for found file line, includes foundFile, fileTime, and fileAttr
     RoutineFileNameBuffer nameSpec;               // Starts out holding the file spec
     FlagSet<OptionFlags, 8> options;              // the options for what we're looking for
     AttributeMask targetAttributes;               // the specific attributes we're looking for
     AttributeMask newAttributes;                  // the specific attributes we're setting
};




#endif

