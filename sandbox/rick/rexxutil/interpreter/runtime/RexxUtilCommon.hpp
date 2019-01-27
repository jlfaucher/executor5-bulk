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
#include "ExternalFileBuffer.hpp"
#include "FlagSet.hpp"

/*********************************************************************/
/* Numeric Error Return Strings                                      */
/*********************************************************************/

#define ERROR_NOMEM "2"
#define ERROR_FILEOPEN "3"


/*********************************************************************/
/* Alpha Numeric Return Strings                                      */
/*********************************************************************/

#define ERROR_RETSTR "ERROR"


/*********************************************************************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/*********************************************************************/

const char* mystrstr(const char *haystack, const char *needle, size_t hlen, size_t nlen, bool sensitive);
inline const char* mystrstr(const char *haystack, const char *needle, bool sensitive = true)
{
    return mystrstr(haystack, needle, strlen(haystack), strlen(needle), sensitive);
}

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

/**
 * Simple class for managing returning results as a stem object.
 */
class StemHandler
{
 public:
     StemHandler(RexxCallContext *c, RexxStemObject s = NULLOBJECT) : context(c), stem(s), arrayCount(0) { }
     ~StemHandler()
     {
         complete();
     }

     void complete()
     {
         if (stem != NULLOBJECT)
         {
             context->SetStemArrayElement(stem, 0, context->StringSizeToObject(arrayCount));
             stem = NULLOBJECT;
         }
     }

     void setStem(const char *stemName)
     {
         // this is a required value...throw an error if not given
         if (stem == NULL)
         {
             context->ThrowException0(Rexx_Error_Incorrect_call);
         }

         char buffer[256];
         strncpy(buffer, stemName, sizeof(buffer));
         if (buffer[strlen(buffer) - 1] != '.')
         {
             strncat(buffer, ".", sizeof(buffer));
         }
         // retrieve the stem object by name from the context
         stem = (RexxStemObject)context->GetContextVariable(buffer);

         if (stem == NULLOBJECT)
         {
             // TODO: flesh this out
             context->ThrowException0(Rexx_Error_Incorrect_call);
         }
     }

     void addList(const char *values)
     {
         // values is a list of null terminated string, terminated with a double null
         while (*values != '\0')
         {
             addValue(values);
             size_t valueLen = strlen(values);
             values += strlen(values) + 1;
         }
     }

     void addValue(const char *value)
     {
         size_t valueLen = strlen(value);
         RexxStringObject s = context->NewString(value, valueLen);
         context->SetStemArrayElement(stem, ++arrayCount, s);
         context->ReleaseLocalReference(s);
     }

 protected:
     RexxCallContext *context; // the call context
     RexxStemObject stem;      // the stem we're managing
     size_t arrayCount;        // the current arrayCount
};


/*
 *  Class to perform SysFileTree functions
 *
 *  Note that in Windows the MAX_PATH define includes the terminating null.
 */
class TreeFinder
{
 public:

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

     /**
      * A set of enumerations for catching exceptions that need to be
      * reported as SysFileTree return values and not as Rexx error conditions.
      */
     typedef enum
     {
         InvalidFileName,          // the filespec failed name validation rules.
     } TreeFinderException;


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

              while (*maskArg != '\0')
              {
                  switch(*maskArg)
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
          char maskMarker(AttributeType maskSetting) { return mask[maskSetting] == AttributeOn ? maskChars[maskSetting] : '-'; }
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

     void findFiles();
     void validateFileSpec();
     void adjustDirectory();
     void validateFileSpecName();
     void parseMask(const char *mask, AttributeMask &flags, size_t argPos);
     void badSFTOptsException(const char *actual);
     void badMaskException(size_t pos, const char *actual);
     void getOptions(const char *opts);
     bool goodOpts(const char *opts);
     void formatFile(const char *fileName);
     void getFullPath();
     void expandNonPath2fullPath();
     void expandPath2fullPath(size_t lastSlashPos);
     void adjustFileSpec();
     void checkFile(const char *fileName);
     void recursiveFindFile(FileNameBuffer &path);
     void addResult(const char *v);
     int findDirectoryEnd();
     bool checkNonPathDrive();
     void fixupFilePath();

     bool includeDirs() { return options[DO_DIRS]; }
     bool includeFiles() { return options[DO_FILES]; }
     bool acceptAll() { return targetAttributes.acceptAll(); }
     bool nameOnly() { return options[NAME_ONLY]; }
     bool longTime() { return options[LONG_TIME]; }
     bool editableTime() { return options[EDITABLE_TIME]; }
     bool longSize() { return options[LONG_SIZE]; }

     bool archiveSelected(bool onOff) { return targetAttributes.isSelected(AttributeMask::Archive, onOff); }
     bool directorySelected(bool onOff) { return targetAttributes.isSelected(AttributeMask::Directory, onOff); }
     bool hiddenSelected(bool onOff) { return targetAttributes.isSelected(AttributeMask::Hidden, onOff); }
     bool readOnlySelected(bool onOff) { return targetAttributes.isSelected(AttributeMask::ReadOnly, onOff); }
     bool systemSelected(bool onOff) { return targetAttributes.isSelected(AttributeMask::System, onOff); }

 protected:

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

