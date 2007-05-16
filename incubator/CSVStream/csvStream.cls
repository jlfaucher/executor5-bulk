/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
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

/* ========================================================================= */
::class CsvStream subclass Stream Public
/* ========================================================================= */
/* ------------------------------------------------------------------------- */
::method FileHasHeaders     Attribute private -- copy of headersExist parm
::method headers            Attribute         -- csvStreamHeader Object
::method originalRawHeaders Attribute private -- for comparison
::method headerLineAbsent   Attribute private -- headersexist but absent
::method OpenArgs           Attribute private -- args to open method
::method values             Attribute         -- table for headered i/o
::method rawText            Attribute         -- copy of last line read
::method skipHeaders        Attribute         -- switch can be set before open
::method lineEnd            Attribute         -- string used for line terminations
::method Delimiter          Attribute         -- seperates CSV fields
::method Qualifier          Attribute         -- Surrounds literals
/* ------------------------------------------------------------------------- */
::method Init
/* ------------------------------------------------------------------------- */
use arg parms, headersExist
                                                        /* initialise values */
self~fileHasHeaders     =   (headersExist = .true                  ),
                          | 'HEADERS'~abbrev(headersexist~translate)
self~skipHeaders        = .true
self~values             = .nil     /* replaced with a table by headered read */
self~OriginalRawHeaders = ''
self~rawText            = ''
self~headerLineAbsent   = .false
self~delimiter          = ','     /* seperates fields in a CSV file          */
self~qualifier          = '"'     /* surrounds literal fields                */
self~openArgs           = .nil

self~init:super(parms)                        /* let stream class initialise */

self~lineEnd            = self~getLineEnd              /* find line end char */

/* ------------------------------------------------------------------------- */
::method Open
/* ------------------------------------------------------------------------- */
use arg args

self~openArgs = args~translate             /* Close needs to know open basis */

if self~fileHasHeaders = .true             /* read the headers into a table  */
then do
   self~headers = .csvStreamHeader~new           /* blank in case no headers */
   if self~open:super('read') = 'READY:'
   then do
      if self~chars > 0
      then do
         self~fileHasHeaders     = .false
         headersArray            = self~CsvLineIn      /* get header array   */
         self~fileHasHeaders     = .true

         do i = 1 to headersArray~last
            if headersArray[i] \= .nil
            then self~headers~field(i)~name = headersArray[i]
         end /* DO */

         self~OriginalRawHeaders = self~rawText
      end /* DO */
      else self~headerLineAbsent = .true
      self~close:super
   end /* DO */
   else self~headerLineAbsent = .true
end /* DO */

forward class (super) continue                              /* open the file */

if   (args~word(1)~translate \= 'WRITE') , /* move read pointer past headers */
&    (self~fileHasHeaders     = .true  ) ,
&    (self~skipHeaders        = .true  ) , /* < user may override this       */
&    (self~chars              > 0      )
then x = self~lineIn:super

/* ------------------------------------------------------------------------- */
::method csvLineIn
/* ------------------------------------------------------------------------- */

if self~openArgs = .nil then self~open

self~rawText  = ''
inLiteral     = .false
fieldNo       = 1
fieldText     = ''
csvFields     = .array~new
literalFields = .set~new
blip          = self~qualifier
blipBlip      = self~qualifier~copies(2)

do until (inLiteral  = .false) ,
   |     (self~chars = 0     )

   text = self~lineIn                              /* get a line of csv text */

   if   self~rawtext = '' ,                        /* not really a csv file  */
   &    text~verify(self~delimiter||self~qualifier,'m')=0
   then text = text||self~delimiter

   if   self~rawText = ''                     /* maintain linetext attribute */
   then self~rawText = text
   else do                                    /* this is a multiline field   */
      self~rawText = self~rawText || self~lineEnd || text
      fieldText    = fieldText    || self~lineEnd
   end /* Do */

   do while text \== ''
      parse var text char 2 text        /* first char of text goes into char */

      select
         when char = self~qualifier
            then do
               inLiteral = 1 - inLiteral
               fieldText = fieldText || char
            end /* DO */

         when (char = self~delimiter | text == '') ,
         &    (\inliteral                        )           /* end of field */
            then do

               if char \= self~delimiter then fieldText = fieldText||char

               /* if field encased in '"'s then strip them */
               if (fieldText~left(1)  = self~qualifier) ,
               &  (fieldText~right(1) = self~qualifier) ,
               &  (fieldText~length   > 1             )
               then do
                  fieldText = fieldText~substr(2, fieldText~length - 2)
                  literalFields~put(fieldNo)
               end /* Do */

               /* '"' are represented in text as '""' */
               fieldText = fieldText~changeStr(blipBlip,blip)

               csvFields[fieldNo] = fieldText
               fieldNo            = fieldNo + 1
               fieldText          = ''
            end /* DO */

         otherwise
            fieldText = fieldText || char
      end /* select */

      if   (text        == '') ,                  /* natural end of row?     */
      &    (\inLiteral       ) ,
      &    (fieldText  \== '')
      then text = self~delimiter                  /* implied field seperator */

   end /* DO */
end /* DO */


if self~fileHasHeaders = .true                     /* create table of values */
then do
   do fieldNo over literalFields
      self~headers~field(fieldNo)~literal= .true
   end /* DO */
   self~values = .table~new
   do i = 1 to self~Headers~last
      if   csvFields[i] = .nil
      then self~values~put('',self~headers~field(i)~name)
      else self~values~put(csvfields[i],self~headers~field(i)~name)
   end /* DO */
end /* DO */

return csvFields

/* ------------------------------------------------------------------------- */
::method csvLineOut
/* ------------------------------------------------------------------------- */
use arg data

blip          = self~qualifier
blipBlip      = self~qualifier~copies(2)

if symbol('DATA') = 'LIT'           /* no parm was passed so close CSVstream */
then do
   self~close
   return
end /* DO */

parse upper value data~class~string with . dataCollectionType .

select
   /* if we have been passed a table or stem, convert it to an array */
   when (self~fileHasHeaders = .true                       ),
   &    .set~of('TABLE','STEM')~hasIndex(dataCollectionType)
   then do
      dataArray = .array~new
      do name over data
         if (name = 0) & (dataCollectionType = 'STEM') then iterate
         column = 0
         do i = 1 to self~headers~last
            if self~headers~field(i)~name = name
            then do
               column = i
               leave
            end /* DO */
         end /* DO */
         if column = 0                                /* unregistered column */
         then do
            column = self~headers~last + 1
            self~headers~field(column)~name = name
         end /* DO */
         dataArray[column] = data[name]
      end /* DO */
      data = dataArray
   end /* DO */

   when dataCollectionType = 'ARRAY'
      then nop

   when data~hasMethod('makeArray')
      then data = data~makearray

   otherwise
      raise syntax 93.953 array (1, "Array")
end /* select */

text = ''
do i = 1 to data~last

   if self~fileHasHeaders            /* force literal field even if numeric? */
   then forceLiteral = (self~Headers~field(i)~literal = .true)
   else forceLiteral = .false

   select
      when data[i] = .nil                                   /*      no value */
         then text = text||self~delimiter
      when ( data[i]~datatype('n') ,                        /* numeric value */
        |    data[i] == ''        ),
      &    \forceLiteral
         then text = text||self~delimiter||data[i]~strip
      when data[i]~left(1)  = self~qualifier ,     /* literal already quoted */
      &    data[i]~right(1) = self~qualifier ,
      &    data[i]~length > 1
      then do
         text = text                                                        ||,
                self~delimiter                                              ||,
                self~qualifier                                              ||,
                data[i]~substr(2,data[i]~length-2)~changeStr(blip,blipBlip) ||,
                self~delimiter
         if   self~fileHasHeaders
         then self~Headers~field(i)~literal = .true
      end /* Do */
      otherwise                                             /* literal value */
         text = text                                                        ||,
                self~delimiter                                              ||,
                self~qualifier                                              ||,
                data[i]~changeStr(blip,blipBlip)                            ||,
                self~qualifier
         if   self~fileHasHeaders
         then self~Headers~field(i)~literal = .true
   end /* select */
end /* DO */

parse var text 2 text                            /* remove leading delimiter */

return self~lineout(text)

/* ------------------------------------------------------------------------- */
::method close
/* ------------------------------------------------------------------------- */

self~close:super

if self~fileHasHeaders
then do                                               /* maintain headers    */
   headerText = ''                                    /* prepare header line */
   do i = 1 to self~headers~last
      if   self~headers~field(i) = .nil
      then headertext = headerText                                          ||,
                        self~delimiter
      else headertext = headerText                                          ||,
                        self~delimiter                                      ||,
                        self~qualifier                                      ||,
                        self~headers~field(i)~name                          ||,
                        self~qualifier
   end /* DO */
   parse var headerText . 2 headerText
   if headerText \= self~originalRawHeaders , /* headers need replacing      */
   &  self~openArgs~word(1) \= 'READ'         /* and file opened for writing */
   then do
      self~open:super('read')
      if self~headerLineAbsent = .false
      then x = self~linein                         /* obsolete header line   */
      entireText = self~charIn(,self~chars)
      self~close:super

      self~open:super('write replace')
      self~lineout(headertext)
      self~charout(entireText)
      self~close:super
   end /* DO */
end /* DO */

/* ------------------------------------------------------------------------- */
::method GetLineEnd
/* ------------------------------------------------------------------------- */
filename = self~qualify

tempfile = sysTempFileName(filespec('drive',filename)||,
                           filespec('path',filename) ||,
                           '$temp??.???')

tfs = .stream~new(tempfile)
tfs~open('write replace')
tfs~lineout('>')
tfs~lineout('<')

body = tfs~charin(1,tfs~chars)
parse var body . '>'crlf'<' .
tfs~close
call sysfiledelete tempfile

return crlf

/* ------------------------------------------------------------------------- */
::method getHeaders
/* ------------------------------------------------------------------------- */
return self~Headers~copy

/* ------------------------------------------------------------------------- */
::method setHeaders
/* ------------------------------------------------------------------------- */
use arg newHeaders

if newHeaders~class~string \= 'The CSVSTREAMHEADER class'
then raise syntax 93.948 array (1, "CsvStreamHeader")

self~Headers = newHeaders~copy

/* ========================================================================= */
::class CsvStreamHeader
/* ========================================================================= */
/* ------------------------------------------------------------------------- */
::method FieldArray    Attribute  Private
/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
self~FieldArray = .array~new

/* ------------------------------------------------------------------------- */
::method field
/* ------------------------------------------------------------------------- */
arg no

if self~fieldArray[no] = .nil
then do
   self~fieldArray[no] = .CsvStreamField~new
   self~fieldArray[no]~name = 'Field' no                     /* default name */
end

return self~FieldArray[no]

/* ------------------------------------------------------------------------- */
::method last
/* ------------------------------------------------------------------------- */
if self~fieldArray~last = .nil
then return 0
else return self~fieldArray~last

/* ========================================================================= */
::class CsvStreamField
/* ========================================================================= */
/* ------------------------------------------------------------------------- */
::method name     Attribute
::method literal  Attribute
/* ------------------------------------------------------------------------- */

/* ========================================================================= */


